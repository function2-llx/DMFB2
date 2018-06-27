#include <set>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>
#include "State.h"
#include "Direction.h"
#include "Grid.h"
#include "Detection.h"
#include "Dispenser.h"
#include "Cell.h"
#include "Cmp.h"
#include "Hash.h"
#include "Tools.h"


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

bool State::canDump(Droplet* droplet)
{
    assert(!droplet->underDetection());
    return !toBeMixed[droplet->getIdentifier()] && droplet->detected();
}
//check if current necessary detection is available
void State::check()
{

}

ULL State::hash()
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

    return false;
}

State sucState;
vector<State*> successors;

State* State::initialState()
{
    State* state = new State();
    state->decision = nullptr;
    state->step = 0;
    return state;
}

bool State::fluidicConstraints(Droplet* droplet)
{

}

int** influnce;
int** occupy;

void State::dfsMove(vector<Droplet*>::iterator it)
{
    if (it == this->droplets.end()) {

    } else {

    }
}

void insert(Droplet* droplet)
{

}

vector<State*> State::getSuccessors()
{
    successors.clear();
    sucState.clear();
    sucState.decision = this;
    sucState.step = this->step + 1; 
    influnce = new int*[grid->getRows()];
    occupy = new int*[grid->getRows()];
    for (int i = 0; i < grid->getRows(); i++) {
        influnce[i] = new int[grid->getColumns()];
        occupy[i] = new int[grid->getColumns()];
        for (int j = 0; j < grid->getColumns(); j++) {
            influnce[i][j] = occupy[i][j] = -1;
        }
    }
    for (auto droplet: this->droplets) {
        Point position(droplet->getPosition());
        int identifier = droplet->getIdentifier();
        for (int i = position.r - 1; i <= position.r + 1; i++) {
            for (int j = position.c - 1; j <= position.c + 1; j++) {
                Point cur(i, j);
                if (grid->inside(cur)) {
                    influnce[i][j] = identifier;
                }
            }
        }
        occupy[position.r][position.c] = identifier;
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
