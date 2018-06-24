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


using namespace std;

State::State(const State& state)
{
    for (auto dispensation: state.dispensations) {
        this->addDispensation(dispensation);
    }
    for (auto droplet: state.droplets) {
        this->addDroplet(new Droplet(*droplet));
    }
    for (auto mixingOperation: state.mixingOperations) {
        this->addMixingOperation(mixingOperation);
    }
    for (auto mixer: state.mixers) {
        this->addMixer(new Mixer(*mixer));
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
    for (auto mixer: this->mixers) {
        delete mixer;
    }
}

void State::clear()
{
    this->droplets.clear();
    this->dispensations.clear();
    this->mixingOperations.clear();
    this->mixers.clear();

}

void State::addDispensation(Dispensation* dispensation)
{
    this->dispensations.push_back(dispensation);
}

void State::removeDispensation()
{
    this->dispensations.pop_back();
}

void State::addDroplet(Droplet* droplet)
{
    this->droplets.push_back(droplet);
}


void State::addMixingOperation(MixingOperation* mixingOperation)
{
    this->mixingOperations.push_back(mixingOperation);
}

void State::removeMixingOperation()
{
    this->mixingOperations.pop_back();
}

void State::addMixer(Mixer* mixer)
{
    this->mixers.push_back(mixer);
}

void State::removeMixer()
{
    this->mixers.pop_back();
}

bool State::canDump(Droplet* droplet)
{
    assert(!droplet->underDetection());
    return !toBeMixed[droplet->getIdentifier()] && droplet->detected();
}
//check if current necessary detection is available
void State::check()
{
    for (auto droplet: this->droplets) {
        for (auto mixer: this->mixers) {
            assert(!mixer->inside(droplet->getPosition()));
        }
    }
    for (auto it = this->mixers.begin(); it != this->mixers.end(); it++) {
        Mixer* mixer = *it;
        for (auto it = this->mixers.begin(); it != this->mixers.end(); it++) {
            Mixer* checkedMixer = *it;
            if (checkedMixer != mixer) {
                assert(!mixer->overlap(checkedMixer));
            }
        }
    }
}

ULL State::hash()
{
    static ULL hashBase = 75487475995782307ull;
    static ULL shift = 3751046701ull;
    ULL ret = 0;
    auto replaceDispensations = this->dispensations;
    sort(replaceDispensations.begin(), replaceDispensations.end(), cmpDispensation);
    for (auto dispensation: replaceDispensations) {
        ret = dispensation->identifier + shift + hashBase * ret;
    }
    auto replaceDroplets = this->droplets;
    sort(replaceDroplets.begin(), replaceDroplets.end(), cmpDroplet);
    for (auto droplet: replaceDroplets) {
        ret = droplet->hash() + shift + hashBase * ret;
    }
    auto replaceMixingOperation = this->mixingOperations;
    sort(replaceMixingOperation.begin(), replaceMixingOperation.end(), cmpMixingOperation);
    for (auto mixingOperation: replaceMixingOperation) {
        ret = mixingOperation->outputIdentifier + shift + hashBase * ret;
    }
    auto replaceMixer = this->mixers;
    sort(replaceMixer.begin(), replaceMixer.end(), cmpMixer);
    for (auto mixer: replaceMixer) {
        ret = mixer->hash() + shift + hashBase * ret;
    }
    return ret;
}

bool State::isEndState()
{
    if (this->dispensations.empty() && this->mixingOperations.empty() && this->mixers.empty()) {
        for (auto droplet: this->droplets) {
            if (!droplet->detected()) {
                return false;
            }
        }
        return true;
    }
    return false;
}

State sucState;
vector<State*> successors;
Droplet** cellDroplet;
set<int> mixedIdentifier;

bool mixed(MixingOperation* mixingOperation)
{
    return mixedIdentifier.count(mixingOperation->mixerIdentifer);
}

void insert(MixingOperation* mixingOperation)
{
    assert(!mixedIdentifier.count(mixingOperation->mixerIdentifer));
    mixedIdentifier.insert(mixingOperation->mixerIdentifer);
}

void erase(MixingOperation* mixingOperation)
{
    auto it = mixedIdentifier.find(mixingOperation->mixerIdentifer);
    assert(it != mixedIdentifier.end());
    mixedIdentifier.erase(it);
}

bool State::canPlace(Mixer* mixer)
{
    for (auto checkMixer: sucState.mixers) {
        if (checkMixer->overlap(mixer)) {
            return false;
        }
    }
    Point upperLeftCorner = mixer->getUpperleftCorner(), lowerRightCorner = mixer->getLowerRightCorner();
    for (int i = upperLeftCorner.r; i <= lowerRightCorner.r; i++) {
        for (int j = upperLeftCorner.c; j <= lowerRightCorner.c; j++) {
            assert(grid->inside(Point(i, j)));
            if (cellDroplet[grid->getPointIdentifier(Point(i, j))] != nullptr) return false;
        }
    }
    return true;
}

State* State::initialState()
{
    State* state = new State();
    state->decision = nullptr;
    for (auto mixingOperation: allMixingOperations) {
        state->addMixingOperation(mixingOperation);
    }
    assert(!allDispensations.empty());
    for (auto dispensation: allDispensations) {
        state->addDispensation(dispensation);
    }
    state->step = 0;
    return state;
}

GridRecorder *preRecorder, *curRecorder;

bool State::fluidicConstraints(Droplet* droplet)
{
    Point position = droplet->getPosition();
    int identifier = droplet->getIdentifier();
    for (auto curDroplet: this->droplets) {
        if (adjacent(droplet->getPosition(), curDroplet->getPosition()) && !mixPair[droplet->getIdentifier()][curDroplet->getIdentifier()]) {
//            cerr << droplet->getPosition() << " " << curDroplet->getPosition() << endl;
        }
    }
    for (int i = position.r - 2; i <= position.r + 2; i++) {
        for (int j = position.c - 2; j <= position.c + 2; j++) {
            Point curPosition(i, j);
            if (grid->inside(curPosition)) {
                int positionIdentifier = grid->getPointIdentifier(curPosition);
                Droplet* curDroplet = cellDroplet[positionIdentifier];
                if (curDroplet != nullptr) {
                    int curIdentifier = curDroplet->getIdentifier();
                    if (!mixPair[identifier][curIdentifier]) return false;
                }
            }
        }
    }
    return true;
}

void State::pushDroplet(vector<Dispensation*>::iterator it, Droplet* droplet)
{
    Point position = droplet->getPosition();
    assert(grid->inside(position));
    for (auto mixer: sucState.mixers) {
        if (mixer->inside(position)) {
            return;
        }
    }
    if (!this->fluidicConstraints(droplet)) return;
    int positionIdentifier = grid->getPointIdentifier(position);
    Droplet* origin = cellDroplet[positionIdentifier];
    if (origin == nullptr) {
        cellDroplet[positionIdentifier] = droplet;
        dfsDispense(it + 1);
    } else if (droplet->detected() && origin->detected()) {
        cellDroplet[positionIdentifier] = nullptr;
        for (auto mixingOperation: this->mixingOperations) {
            if (mixingOperation->match(origin->getIdentifier(), droplet->getIdentifier())) {
                insert(mixingOperation);
                int r1 = max(0, position.r - mixingOperation->rows + 1);
                int r2 = min(position.r, grid->getRows() - mixingOperation->rows);
                int c1 = max(0, position.c - mixingOperation->columns + 1);
                int c2 = min(position.c, grid->getColumns() - mixingOperation->columns);
                for (int i = r1; i <= r2; i++) {
                    for (int j = c1; j <= c2; j++) {
                        Mixer* mixer = new Mixer(mixingOperation, position, Point(i, j));
                        if (sucState.canPlace(mixer)) {
                            sucState.addMixer(mixer);
                            dfsDispense(it + 1);
                            sucState.removeMixer();
                        }
                        delete mixer;
                    }
                }
                erase(mixingOperation);
            }
        }
    }
    cellDroplet[grid->getPointIdentifier(position)] = origin;
}

void State::dfsDispense(vector<Dispensation*>::iterator it)
{
    if (it == this->dispensations.end()) {
        State* state = new State(sucState);
        for (int i = 0; i < grid->area(); i++) {
            if (cellDroplet[i] != nullptr) {
                for (auto mixer: state->mixers) {
                    assert(!mixer->inside(cellDroplet[i]->getPosition()));
                }
                state->addDroplet(new Droplet(*cellDroplet[i]));
            }
        }
        for (auto mixingOperation: this->mixingOperations) {
            if (!mixed(mixingOperation)) {
                state->addMixingOperation(mixingOperation);
            }
        }
        state->check();
        ULL hash = state->hash();
        if (!hashSet.count(hash)) {
            hashSet.insert(hash);
			state->estimatedTime();
            successors.push_back(state);
        } else {
            delete state;
        }
    } else {
		Dispensation* dispensation = *it;
        Droplet* droplet = new Droplet(dispenser[dispensation->type], dispensation);
        this->pushDroplet(it, droplet);
        delete droplet;
		sucState.addDispensation(dispensation);
		this->dfsDispense(it + 1);
		sucState.removeDispensation();        
    }
}

void State::pushDroplet(vector<Droplet*>::iterator it, Droplet* droplet)
{
    Point position = droplet->getPosition();
    for (auto mixer: sucState.mixers) {
        if (mixer->inside(position)) {
            return;
        }
    }
    if (!this->fluidicConstraints(droplet)) return;
    assert(grid->inside(position));
    int positionIdentifier = grid->getPointIdentifier(position);
    Droplet* origin = cellDroplet[positionIdentifier];
    if (origin == nullptr) {
        cellDroplet[positionIdentifier] = droplet;
        dfsMove(it + 1);
    } else if (droplet->detected() && origin->detected()) {
        cellDroplet[positionIdentifier] = nullptr;
        for (auto mixingOperation: this->mixingOperations) {
            if (mixingOperation->match(origin->getIdentifier(), droplet->getIdentifier())) {
                insert(mixingOperation);
                int r1 = max(0, position.r - mixingOperation->rows + 1);
                int r2 = min(position.r, grid->getRows() - mixingOperation->rows);
                int c1 = max(0, position.c - mixingOperation->columns + 1);
                int c2 = min(position.c, grid->getColumns() - mixingOperation->columns);
                for (int i = r1; i <= r2; i++) {
                    for (int j = c1; j <= c2; j++) {
                        Mixer* mixer = new Mixer(mixingOperation, position, Point(i, j));
                        if (sucState.canPlace(mixer)) {
                            sucState.addMixer(mixer);
                            dfsMove(it + 1);
                            sucState.removeMixer();
                        }
                        delete mixer;
                    }
                }
                erase(mixingOperation);
            }
        }
    }
    cellDroplet[grid->getPointIdentifier(position)] = origin;
}

int cnt = 0;

void State::dfsMove(vector<Droplet*>::iterator it)
{
    if (it == this->droplets.end()) {
        this->dfsDispense(this->dispensations.begin());
    } else {
        Droplet* droplet = *it;
        if (droplet->underDetection()) { //must stay still if is being detected
            Droplet* newDroplet = new Droplet(droplet, zeroDirection);
            this->pushDroplet(it, newDroplet);
            delete newDroplet;
        } else {
            Point position = droplet->getPosition();
            Cell* cell = grid->getCell(position);
            if (!droplet->detected() && cell->existDetector()) {
                Detector* detector = cell->getDetector();
                int type = droplet->getType();           
                int type2 = detector->getType();                  
                if (type == type2) {
                    Droplet* newDroplet = new Droplet(droplet, zeroDirection);
                    newDroplet->startDetection();
                    this->pushDroplet(it, newDroplet);
                    delete newDroplet;
                }
            }
            if (cell->existSink() && this->canDump(droplet)) {
                dfsMove(it + 1);
            } else {
                for (int i = 0; i < 5; i++) {
                    if (grid->inside(position + direction[i])) {
                        Droplet* newDroplet = new Droplet(droplet, direction[i]);
                        pushDroplet(it, newDroplet);
                        delete newDroplet;
                    }
                }
            }
        }
    }
}

vector<State*> State::getSuccessors()
{
    preRecorder = new GridRecorder(grid->getRows(), grid->getColumns());
    curRecorder = new GridRecorder(grid->getRows(), grid->getColumns());
    for (auto droplet: this->droplets) {
        preRecorder->addDroplet(droplet);
    }
    for (auto mixer: this->mixers) {
        preRecorder->addMixer(mixer);
    }
    successors.clear();
    sucState.clear();
    sucState.decision = this;
    sucState.step = this->step + 1; 
    cellDroplet = new Droplet*[grid->area()];
    for (int i = 0; i < grid->area(); i++) {
        cellDroplet[i] = nullptr;
    }
    vector<Mixer*> newMixers;
    newMixers.clear();
    for (auto mixer: this->mixers) {  //deal with previous mixers
        if (mixer->getRemainingTime() == 1) {
            cellDroplet[grid->getPointIdentifier(mixer->getOutputPosition())] = new Droplet(mixer);
            assert(sucState.mixers.size() == 0);
        } else {
            Mixer* newMixer= new Mixer(mixer);
            sucState.addMixer(newMixer);
            newMixers.push_back(newMixer);
        }
    }
    mixedIdentifier.clear();
    this->dfsMove(this->droplets.begin());
    for (auto mixer: newMixers) {
        delete mixer;
    }
    for (int i = 0; i < grid->area(); i++) {
        if (cellDroplet[i] != nullptr) {
            delete cellDroplet[i];
        }
    }
    delete []cellDroplet;
    delete preRecorder;
    delete curRecorder;
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
        for (auto mixer: this->mixers) {
            ret = max(ret, mixer->estimatedTime());
        }
        for (auto dispensation: this->dispensations) {
            ret = max(ret, dispensation->estimatedTime());
        }
    }
    return ret;
}

bool State::operator < (const State& state) const
{
    return this->estimation < state.estimation;
}

void State::vitualPrint(ostream& os)
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
    for (auto mixer: this->mixers) {
        Point p1 = mixer->getUpperleftCorner(), p2 = mixer->getLowerRightCorner();
        for (int i = p1.r; i <= p2.r; i++) {
			for (int j = p1.c; j <= p2.c; j++) {
				type[i][j] = 10 + mixer->getIdentifier();
			}
		}
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
    this->vitualPrint(os);

    for (auto dispensation: this->dispensations) {
        os << *dispensation << endl;
    }
    for (auto droplet: this->droplets) {
        os << *droplet << endl;
    }
    for (auto mixingOperation: this->mixingOperations) {
        os << *mixingOperation << endl;
    }
    for (auto mixer: this->mixers) {
        os << *mixer << endl;
    }
    os << "current step: " << this->step << endl;
    os << "estimated eventually step: " << this->step + this->estimatedTime() << endl;
    os << "======================================" << endl;
}

void State::textPrint(ostream& os)
{
    os << "==================State================" << endl;
    for (auto dispensation: this->dispensations) {
        os << *dispensation << endl;
    }
    for (auto droplet: this->droplets) {
        os << *droplet << endl;
    }
    for (auto mixingOperation: this->mixingOperations) {
        os << *mixingOperation << endl;
    }
    for (auto mixer: this->mixers) {
        os << *mixer << endl;
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
