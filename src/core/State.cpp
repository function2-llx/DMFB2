#include "core/State.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
#include "Global.h"
#include "core/DMFB.h"
#include "entities/Dispenser.h"
#include "grid/Cell.h"
#include "grid/Grid.h"
#include "math_models/Direction.h"

using namespace std;

State::State(const State *precursor) {
    this->estimation = 0;
    this->step = precursor->step + 1;
    this->decision = precursor;
}

State::State(const State &state) {
    for (auto droplet : state.droplets)
        this->droplets.push_back(new Droplet(*droplet));

    this->estimation = state.estimation;
    this->step = state.step;
    this->decision = state.decision;
}

State::~State()
{
    for (auto droplet : this->droplets)
        delete droplet;
}

bool operator == (const State& a, const State& b) 
{
    if (a.droplets.size() != b.droplets.size()) return 0;

    for (int i = a.droplets.size() - 1; i >= 0; i--) {
        if (*a.droplets[i] != *b.droplets[i]) return 0;
    }

    return 1;
}

void State::addDroplet(const Droplet *droplet) {
    this->droplets.push_back(droplet);
    this->estimation = max(estimation, droplet->estimatedTime());
}

State::State() {
    this->step = 0;
    this->estimation = 0;
    this->decision = nullptr;
    for (int id : DMFBsolver->get_dispense_id())
        addDroplet(new Droplet(DMFBsolver->get_droplet_data(id)));
}

vector<const Droplet *> State::getDroplets() const { return this->droplets; }

void State::clean() const {
    if (this->decision != nullptr) this->decision->clean();

    delete this;
}

ULL State::hash() const {
    constexpr static ULL hashBase = 1231612771ull;
    constexpr static ULL shift = 99426053ull;
    ULL ret = 79451089ull;

    for (auto droplet : droplets)
        ret = droplet->hash() + shift + hashBase * ret;

    return ret;
}

bool State::isEndState() const {
    for (auto droplet : this->droplets) {
        if (!droplet->mixed() || !droplet->detected() ||
            !droplet->isEndDroplet()) {
            return false;
        }
    }
    return true;
}

int State::estimationTime() const { return this->estimation; }

static vector<const State *> successors;

static int **curInfluence, **preInfluence;
static vector<Droplet> **content;            // record droplets in every grid
static vector<const Droplet *> undispensed;  // record undispensed droplets

static bool canPush(const Droplet &droplet) {
    using namespace Global;
    int identifier = droplet.get_id();
    Point position = droplet.getPosition();
    int pre = preInfluence[position.r][position.c],
        cur = curInfluence[position.r][position.c];
    if (pre != -1 && pre != identifier) {
        if (!droplet.mixed() || !droplet.detected()) return false;
        // if (mixingResult[pre][identifier] == -1) return false;
        if (DMFBsolver->get_mixing_result_id(pre, identifier) == -1) return 0;
    }
    if (cur != -1) {
        assert(cur != identifier);
        if (!droplet.mixed() || !droplet.detected()) return false;
        // if (mixingResult[cur][identifier] == -1) return false;
        if (DMFBsolver->get_mixing_result_id(cur, identifier) == -1) return 0;
    }

    return true;
}

void State::pushDroplet(const Droplet &droplet, unsigned int number) const {
    int identifier = droplet.get_id();
    Point position = droplet.getPosition();
    if (canPush(droplet)) {
        // cerr << identifier << ' ' << position << endl;
        content[position.r][position.c].push_back(droplet);
        int record[3][3];
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                Point cur(position.r + i, position.c + j);
                if (grid->inside(cur)) {
                    record[i + 1][j + 1] = curInfluence[cur.r][cur.c];
                    curInfluence[cur.r][cur.c] = identifier;
                }
            }
        }
        this->dfsMove(number + 1);
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                Point cur(position.r + i, position.c + j);
                if (grid->inside(cur)) {
                    curInfluence[cur.r][cur.c] = record[i + 1][j + 1];
                }
            }
        }
        content[position.r][position.c].pop_back();
    }
}

void State::sort_droplets()
{
    sort(droplets.begin(), droplets.end(), [] (const Droplet* a, const Droplet* b) {
        return a->get_id() < b->get_id();
    });
}

void State::dfsMove(unsigned int number) const {
    if (number == this->droplets.size()) {
        State *successor = new State(this);
        for (unsigned int i = 0; i < undispensed.size(); i++) {
            successor->addDroplet(new Droplet(*undispensed[i]));
        }
        for (int i = 0; i < grid->getRows(); i++) {
            for (int j = 0; j < grid->getColumns(); j++) {
                if (content[i][j].size() == 1) {
                    // cerr << "single id: " << content[i][j][0].get_id() << endl;
                    successor->addDroplet(new Droplet(content[i][j][0]));
                }
                if (content[i][j].size() == 2) {
                    successor->addDroplet(new Droplet(content[i][j][0], content[i][j][1]));
                }
            }
        }

        successor->sort_droplets();

        // cerr << "droplets size: " << successor->getDroplets().size() << endl;
        // cerr << *successor << endl;

#ifdef UNIQUE
        ULL hash = successor->hash();
        if (!hashSet.count(hash)) {
            hashSet.insert(hash);
            // cerr << hashSet.size() << endl;
            successors.push_back(successor);
        } else {
            delete successor;
        }
#else
        successors.push_back(successor);
#endif
    } else {
        const Droplet *droplet = droplets[number];
        int identifier = droplet->get_id();
        Point position = droplet->getPosition();

        if (!droplet->is_dispensed()) {  // deal with undispensed droplet
            this->pushDroplet(Droplet(droplet, zeroDirection),
                              number);  // dispense and continue dfs implicitly

            undispensed.push_back(droplet);  // still not dispense
            this->dfsMove(number + 1);
            undispensed.pop_back();
        } else if (droplet->underMixing()) {  // droplet under mixing must move(?)
            for (int i = 0; i < 5; i++) {
                if (direction[i] != zeroDirection && grid->pos_available(position + direction[i])) {
                    this->pushDroplet(Droplet(droplet, direction[i]), number);
                }
            }
        } else if (droplet->underDetection()) {  // droplet under detection must stay still
            this->pushDroplet(Droplet(droplet, zeroDirection), number);
        } else {                        // free droplet
            if (droplet->detected()) {  // attempt to dump into sink
                if (!DMFBsolver->is_to_mix(droplet)) {
                    Cell *cell = grid->get_cell(position);
                    if (cell->existSink())
                        this->dfsMove(number + 1);
                }
            } else {  // attempt to start detection
                Cell *cell = grid->get_cell(position);
                if (cell->existDetector() &&
                    cell->getDetector()->get_type() == droplet->getType()) {
                    Droplet newDroplet(droplet, zeroDirection);
                    newDroplet.startDetection();
                    newDroplet.time_past(1);
                    this->pushDroplet(newDroplet, number);
                }
            }
            for (int i = 0; i < 5; i++) {  // trivial move
                if (grid->pos_available(position + direction[i])) {
                    pushDroplet(Droplet(droplet, direction[i]), number);
                }
            }
        }
    }
}

vector<const State *> State::get_successors() const {
    successors.clear();
    preInfluence = new int *[grid->getRows()];
    curInfluence = new int *[grid->getRows()];
    content = new vector<Droplet> *[grid->getRows()];
    for (int i = 0; i < grid->getRows(); i++) {
        preInfluence[i] = new int[grid->getColumns()];
        curInfluence[i] = new int[grid->getColumns()];
        content[i] = new vector<Droplet>[grid->getColumns()];
        for (int j = 0; j < grid->getColumns(); j++) {
            curInfluence[i][j] = preInfluence[i][j] = -1;
        }
    }
    for (auto droplet : this->droplets) {
        if (droplet->is_dispensed()) {
            Point position(droplet->getPosition());
            int identifier = droplet->get_id();
            for (int i = position.r - 1; i <= position.r + 1; i++) {
                for (int j = position.c - 1; j <= position.c + 1; j++) {
                    if (grid->inside(Point(i, j))) {
                        preInfluence[i][j] = identifier;
                    }
                }
            }
        }
    }

    this->dfsMove(0);

    for (int i = 0; i < grid->getRows(); i++) {
        delete[] preInfluence[i];
        delete[] curInfluence[i];
        delete[] content[i];
    }
    delete[] preInfluence;
    delete[] curInfluence;
    delete[] content;
    // cerr << successors.size() << endl;
    return successors;
}

std::ostream &operator << (ostream &os, const State &state) {
    state.allPrint(os);
    return os;
}

void State::visualPrint(ostream &os) const {
    os << "step " << this->step << endl;
    int type[grid->getRows()][grid->getColumns()];
    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getColumns(); j++) {
            type[i][j] = -1;
        }
    }
    for (auto droplet : this->droplets) {
        if (droplet->is_dispensed()) {
            Point position = droplet->getPosition();
            type[position.r][position.c] = DMFBsolver->get_real_type(droplet->getType());
            // ::type[droplet->getType()];
        }
    }
    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getColumns(); j++) {
            if (type[i][j] == -1) {
                if (grid->pos_available(Point(i, j)))
                    os << "N  ";
                else
                    os << "X  ";
            } else {
                os << "D" << type[i][j] << " ";
            }
        }
        os << endl;
    }
}

void State::allPrint(ostream &os) const {
    os << "=================State=================" << endl;
    this->visualPrint(os);

    if (!droplets.empty()) {
        os << endl << "droplet state: " << endl << endl;
        for (auto droplet : this->droplets) {
            if (droplet->is_dispensed()) {
                os << *droplet << endl;
            }
        }
    }
    os << "current step: " << this->step << endl;
    os << "estimated eventually step: " << this->step + this->estimation
       << endl;
    os << "======================================" << endl;
}

void State::textPrint(ostream &os) const {
    os << "=================State=================" << endl;
    for (auto droplet : this->droplets) {
        if (droplet->is_dispensed()) {
            os << *droplet << endl;
        }
    }
    os << "current step: " << this->step << endl;
    os << "estimated eventually step: " << this->step + this->estimation
       << endl;
    os << "======================================" << endl;
}

void State::printSolution(ostream &os) const {
    assert(this != nullptr);
    if (this->decision != nullptr) {
        this->decision->printSolution(os);
    }
    os << *this << endl;
}

#include <stack>

std::vector<const State*> State::get_whole_route(const State* start, const State* end)
{
    using namespace std;

	stack<const State*> stk;
	for (auto state = end; ; state = state->decision) {
		stk.push(state);
		if (state == start)
			break;
	}

	vector<const State*> route;
	route.reserve(stk.size());
	for (int i = stk.size() - 1; i >= 0; i--) {
		route.push_back(stk.top());
		stk.pop();
	}

	return route;
}
