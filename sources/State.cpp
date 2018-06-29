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

State::State()
{
    this->estimation = -1;
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
}

bool State::canDump(const Droplet* droplet) const
{
    assert(!droplet->underDetection());
    return !Global::toBeMixed[droplet->getIdentifier()] && droplet->detected();
}
//check if current necessary detection is available
void State::check()
{

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

bool State::isEndState()
{
    for (auto droplet: this->droplets) {
        if (!droplet->detected() || !droplet->isEndDroplet()) return false;
    }
    return true;
}

State sucState;
vector<State*> successors;

State* State::initialState()
{
    using namespace Global;
    State* state = new State();
    state->decision = nullptr;
    state->step = 0;
    for (int i = 0; i < nDroplets; i++) {
        if (toBeDispensed[i]) {
            state->addDroplet(new Droplet(dropletData[i]));
        }
    }
    return state;
}

bool State::fluidicConstraints(Droplet* droplet)
{
    
}

int **curInfluence, **preInfluence;
vector<Droplet> **content;  //record droplets in every grid
vector<Droplet*> undispensed;   //record undispensed droplets

bool check(int identifier, Point position)
{
    using namespace Global;
    int pre = preInfluence[position.r][position.c];
    if (pre != -1 && !mixPair[pre][identifier]) return false;
    int cur = curInfluence[position.r][position.c];
    if (cur != -1  && !mixPair[cur][identifier]) return false;
    return true;
}

void State::dfsMove(const vector<Droplet*>::iterator it) const
{
    if (it == this->droplets.end()){
        
    } else {
        Droplet* droplet = *it;
        int identifier = droplet->getIdentifier();
        Point position = droplet->getPosition();
        if (!droplet->inGrid()) {   //deal with undispensed droplet
            if (::check(identifier, position)) {
                Droplet newDroplet(droplet, zeroDirection);
                content[position.r][position.c].push_back(newDroplet);
                int record[3][3];
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (grid->inside(Point(position.r + i, position.c + j))) {
                            record[i + 1][j + 1] = curInfluence[position.r + i][position.c + j];
                        }
                    }
                }
                this->dfsMove(it + 1);
                for (int i = -1; i <= 1; i++) {
                    for (int j = -1; j <= 1; j++) {
                        if (grid->inside(Point(position.r + i, position.c + j))) {
                            curInfluence[position.r + i][position.c + j] = record[i + 1][j + 1];
                        }
                    }
                }
                content[position.r][position.c].pop_back();
            }
            undispensed.push_back(droplet);
            dfsMove(it + 1);
            undispensed.pop_back();
        } else if (droplet->underMixing()) {

        } else if (droplet->underDetection()) {

        } else {
            
        }
    }
}

vector<State*> State::getSuccessors()
{
    successors.clear();
    sucState.clear();
    sucState.decision = this;
    sucState.step = this->step + 1; 
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
    this->dfsMove(this->droplets.begin());
    return successors;
}

void State::clean()
{
    if (this->decision != nullptr) {
        this->decision->clean();
    }
    delete this;
}

ostream& operator << (ostream& os, State& state)
{
    state.allPrint(os);
	return os;
}

int State::estimatedTime()
{
    int& ret = this->estimation;
    if (ret == -1) {
        for (auto droplet: this->droplets) {
            ret = max(ret, droplet->estimatedTime());
        }
    }
    return ret;
}

bool State::operator < (const State& state) const
{
    return this->estimation < state.estimation;
}

void State::visualPrint(ostream& os)
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

void State::allPrint(ostream& os)
{
    os << "==================State================" << endl;
    this->visualPrint(os);

    for (auto droplet: this->droplets) {
        os << *droplet << endl;
    }
    os << "current step: " << this->step << endl;
    os << "estimated eventually step: " << this->step + this->estimatedTime() << endl;
    os << "======================================" << endl;
}

void State::textPrint(ostream& os)
{
    os << "==================State================" << endl;
    for (auto droplet: this->droplets) {
        os << *droplet << endl;
    }
    os << "current step: " << this->step << endl;
    os << "estimated eventually step: " << this->step + this->estimatedTime() << endl;
    os << "======================================" << endl;
}

void State::printSolution(ostream& os)
{
    assert(this != nullptr);
    if (this->decision != nullptr) {
        this->decision->printSolution(os);
    }
	os << *this << endl;
}
