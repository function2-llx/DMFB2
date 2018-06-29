#include <set>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include "State.h"
#include "Direction.h"
#include "Grid.h"
#include "Dispenser.h"
#include "Cell.h"
#include "Cmp.h"
#include "Hash.h"
#include "Global.h"

using namespace std;

State::State(const State& state)
{
    for (auto droplet: state.droplets) {
        this->addDroplet(new Droplet(*droplet));
    }
    this->step = state.step;
    this->decision = state.decision;
    this->estimation = -1;
}

State::State(const State* precursor)
{
    this->estimation = -1;
	this->step = precursor->step + 1;
	this->decision = precursor;
}

State::~State()
{
    for (auto droplet: this->droplets) {
        delete droplet;
    }
}

void State::clear()
{
    this->droplets.clear();
}

void State::addDroplet(Droplet* droplet)
{
    this->droplets.push_back(droplet);
    this->estimation = max(estimation, droplet->estimatedTime());
}

State::State()
{
    using namespace Global;
    this->decision == nullptr;
    this->step = 0;
    for (int i = 0; i < nDroplets; i++) {
        if (toBeDispensed[i]) {
            this->addDroplet(new Droplet(dropletData[i]));
        }
    }
}

//check if current necessary detection is available
void State::check() const
{

}

void State::clean() const
{
    if (this->decision != nullptr) {
        this->decision->clean();
    }
    delete this;
}

ULL State::hash() const
{
    static ULL hashBase = 75487475995782307ull;
    static ULL shift = 3751046701ull;
    ULL ret = 0;
    auto replaceDroplets = this->droplets;
    sort(replaceDroplets.begin(), replaceDroplets.end(), cmpDroplet);
    for (auto droplet: replaceDroplets) {
        ret = droplet->hash() + shift + hashBase * ret;
    }
    return ret;
}

bool State::isEndState() const
{
    for (auto droplet: this->droplets) {
        if (!droplet->detected() || !droplet->isEndDroplet()) return false;
    }
    return true;
}

int State::estimationTime() const
{
    return this->estimation;
}

vector<const State*> successors;


int **curInfluence, **preInfluence;
vector<Droplet> **content;  //record droplets in every grid
vector<const Droplet*> undispensed;   //record undispensed droplets

bool canPush(const Droplet& droplet)
{
    using namespace Global;
    int identifier = droplet.getIdentifier();
    Point position = droplet.getPosition();
    int pre = preInfluence[position.r][position.c], cur = curInfluence[position.r][position.c];
    if (pre != -1 || cur != -1) {
        if (droplet.underMixing() || !droplet.detected()) return false;
        if (pre != -1 && mixingResult[pre][identifier] == -1) return false;
        if (cur != -1 && mixingResult[cur][identifier] == -1) return false;
    }
    return true;
}

void State::pushDroplet(const Droplet& droplet, unsigned int number) const
{
    int identifier = droplet.getIdentifier();
    Point position = droplet.getPosition();
    if (canPush(droplet)) {
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

void State::dfsMove(unsigned int number) const
{
    if (number == this->droplets.size()){
		State* state;
        for (int i = 0; i < grid->getRows(); i++) {
			for (int j = 0; j < grid->getColumns(); j++) {
				if (content[i][j].size() == 1) {

				}
			}
		}
    } else {
        const Droplet* droplet = droplets[number];
        int identifier = droplet->getIdentifier();
        Point position = droplet->getPosition();
        if (!droplet->inGrid()) {   //deal with undispensed droplet
            this->pushDroplet(Droplet(droplet, zeroDirection), number);
            undispensed.push_back(droplet);
            this->dfsMove(number + 1);
            undispensed.pop_back();
        } else if (droplet->underMixing()) {    //droplet under mixing must move(?)
            for (int i = 0; i < 5; i++) {
                if (direction[i] != zeroDirection) {
                    this->pushDroplet(Droplet(droplet, direction[i]), number);
                }
            }
        } else if (droplet->underDetection()) { //droplet under detection must stay still
            this->pushDroplet(Droplet(droplet, zeroDirection), number);
        } else {    //free droplet
            if (droplet->detected()) {  //attempt to dump into sink
				if (!Global::toBeMixed[identifier]) {
					Cell *cell = grid->getCell(position);
					if (cell->existSink()) {
                        this->dfsMove(number + 1);
					}
				}
            } else {    //attempt to start detection
                Cell *cell = grid->getCell(position);
                if (cell->existDetector() && cell->getDetector()->getType() == droplet->getType()) {
                    Droplet newDroplet(droplet, zeroDirection);
                    newDroplet.startDetection();
                    this->pushDroplet(newDroplet, number);
                }                
            }
            for (int i = 0; i < 5; i++) {   //trivial move
                if (grid->inside(position + direction[i])) {
                    pushDroplet(Droplet(droplet, direction[i]), number);
                }
            }
        }
    }
}

vector<const State*> State::getSuccessors() const
{
    successors.clear();
    preInfluence = new int*[grid->getRows()];
    content = new vector<Droplet>*[grid->getRows()];
    for (int i = 0; i < grid->getRows(); i++) {
        preInfluence[i] = new int[grid->getColumns()];
        curInfluence[i] = new int[grid->getColumns()];
        for (int j = 0; j < grid->getColumns(); j++) {
            curInfluence[i][j] = preInfluence[i][j] = -1;
        }
    }
    for (auto droplet: this->droplets) {
        Point position(droplet->getPosition());
        int identifier = droplet->getIdentifier();
        for (int i = position.r - 1; i <= position.r + 1; i++) {
            for (int j = position.c - 1; j <= position.c + 1; j++) {
                if (grid->inside(Point(i, j))) {
                    preInfluence[i][j] = identifier;
                }
            }
        }
    }
    auto it = this->droplets.begin();
    this->dfsMove(0);
    return successors;
}

ostream& operator << (ostream& os, const State& state)
{
    state.allPrint(os);
	return os;
}

bool State::operator < (const State& state) const
{
    return this->estimation < state.estimation;
}

void State::visualPrint(ostream& os) const
{
	os << "step " << this->step << endl;
	int type[10][10];
    for (int i = 0; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getColumns(); j++) {
            type[i][j] = -1;
        }
    }
    for (auto droplet: this->droplets) {
        int r, c;
        droplet->getPosition().getData(r, c);
        type[r][c] = droplet->getIdentifier();
    }
    for (int i = 0 ; i < grid->getRows(); i++) {
        for (int j = 0; j < grid->getColumns(); j++) {
			if (type[i][j] == -1) {
				os << "N  ";
			} else if (type[i][j] < 10) {
				os << "D" << type[i][j] << " ";
			} else {
				os << "M" << type[i][j] - 10 << " ";
			}
        }
		os << endl;
    }
}

void State::allPrint(ostream& os) const
{
    os << "==================State================" << endl;
    this->visualPrint(os);

    for (auto droplet: this->droplets) {
        os << *droplet << endl;
    }
    os << "current step: " << this->step << endl;
    os << "estimated eventually step: " << this->step + this->estimation << endl;
    os << "======================================" << endl;
}

void State::textPrint(ostream& os) const
{
    os << "==================State================" << endl;
    for (auto droplet: this->droplets) {
        os << *droplet << endl;
    }
    os << "current step: " << this->step << endl;
    os << "estimated eventually step: " << this->step + this->estimation << endl;
    os << "======================================" << endl;
}

void State::printSolution(ostream& os) const
{
    assert(this != nullptr);
    if (this->decision != nullptr) {
        this->decision->printSolution(os);
    }
	os << *this << endl;
}
