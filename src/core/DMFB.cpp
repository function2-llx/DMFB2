#include <set>
#include <map>
#include <queue>
#include <cassert>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "Global.h"
#include "entities/Dispenser.h"
#include "grid/Grid.h"
#include "core/State.h"
#include "core/DMFB.h"
#include "core/Map.h"
#include "useless/PlaceState.h"

using namespace std;

struct Node {
	int identifier, type;
	Node* fa;
	Node* ch[2];
	Node() {
		fa = nullptr;
		ch[0] = ch[1] = nullptr;
	}
	inline void insertChild(Node* child)
	{
		if (this->ch[0] == nullptr) {
			this->ch[0] = child;
		} else {
			assert(this->ch[1] == nullptr);
			this->ch[1] = child;
		}
	}
};

int DMFB::getDropletNumber() const { return this->nDroplets; }

Node** node;

DMFB::DMFB()
{
	this->typeMap.clear();
	this->nTypes = 0;
	this->nDispensers = 0;
}

DMFB::~DMFB()
{
	for (int i = 0; i < 4; i++)
		delete []this->boundary_record[i];

	for (int i = 0; i < this->rows; i++)
		delete []this->detector_record[i];

	delete []this->detector_record;
}


static void dfs_least(Node* cur, std::vector<int>& least_time, const std::vector<DropletData>& droplet_data)
{
	if (cur == nullptr) 
		return;
	if (cur->fa != nullptr) 
		least_time[cur->identifier] += least_time[cur->fa->identifier] + 
            droplet_data[cur->fa->identifier].mixingTime + 
            droplet_data[cur->fa->identifier].detectingTime;
	for (int i = 0; i < 2; i++)
		dfs_least(cur->ch[i], least_time, droplet_data);
}

vector<int> type;

void DMFB::loadSequencingGraph()
{
	using namespace Global;

	ifstream is("./input/SequencingGraph.txt");
	assert(is.is_open());

	is >> this->nDroplets;
	node = new Node*[this->nDroplets];
	to_mix.resize(nDroplets, 0);
	// toBeDispensed = new bool[this->nDroplets];
	mixing_result.resize(nDroplets, vector<int>(nDroplets, -1));
    droplet_data.resize(nDroplets);
	for (int i = 0; i < this->nDroplets; i++)
		node[i] = new Node();
	for (int i = 0; i < this->nDroplets; i++) {
		node[i]->identifier = i;
		// dropletData[i].identifier = i;
        droplet_data[i].identifier = i;
		is >> node[i]->type;
		int fatherIndentifier;
		is >> fatherIndentifier;
		if (fatherIndentifier != 0) {
			fatherIndentifier--;
			node[i]->fa = node[fatherIndentifier];
			node[fatherIndentifier]->insertChild(node[i]);
		}
		is >> droplet_data[i].mixingTime >> droplet_data[i].detectingTime;
	}
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->fa == nullptr)
			to_mix[i] = false;
		else
            to_mix[i] = true;
	}
	for (int i = 0 ; i < this->nDroplets; i++) {
		if (node[i]->ch[0] == nullptr) {
			assert(droplet_data[i].mixingTime  == 0);
			// to_dispense[i] = true;
            dispense_id.push_back(i);
			assert(node[i]->ch[1] == nullptr);
			if (!this->typeMap.count(node[i]->type)) {
				this->typeMap[node[i]->type] = this->nDispensers++;
				type.push_back(node[i]->type);
			}
			node[i]->type = this->typeMap[node[i]->type];
		} 
        // else {
		// 	to_dispense[i] = false;
		// }
	}
    
	// dispenser = new Dispenser*[this->nDispensers];
	// for (int i = 0; i < this->nDispensers; i++) 
	// 	dispenser[i] = new Dispenser(i);
    dispensers.reserve(nDispensers);
    for (int i = 0; i < nDispensers; i++)
        dispensers.push_back(new Dispenser(i));

	this->nTypes = this->nDispensers;
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->ch[0] != nullptr) {
			assert(node[i]->ch[1] != nullptr);
			if (!this->typeMap.count(node[i]->type)) {
				this->typeMap[node[i]->type] = this->nTypes++;
				type.push_back(node[i]->type);
			}
			node[i]->type = this->typeMap[node[i]->type];
		}
		droplet_data[i].type = node[i]->type;
	}
	// detectorPosition = new Point[this->nTypes];

	//ensure same input types output same type
	struct data {
		int a, b;
		data(int a, int b)
		{
			if (a > b) swap(a, b);
			this->a = a;
			this->b = b;
		}
		bool operator < (const data& other) const
		{
			if (this->a == other.a) {
				return this->b < other.b;
			}
			return this->a < other.a;
		}
	};	
	map<data, int> mixType;
	mixType.clear();
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->fa == nullptr)
			to_mix[i] = false;
		else 
			// toBeMixed[i] = true;
            to_mix[i] = 1;
		if (node[i]->ch[0] == nullptr) {
			assert(node[i]->ch[0] == nullptr);
		} else {
			Node *ch1 = node[i]->ch[0], *ch2 = node[i]->ch[1];
			assert(ch2 != nullptr);
			mixing_result[ch1->identifier][ch2->identifier] = mixing_result[ch2->identifier][ch1->identifier] = i;
			data cur(ch1->type, ch2->type);
			if (mixType.count(cur)) {
				assert(node[i]->type == mixType[cur]);
			} else {
				mixType[cur] = node[i]->type;
			}
		}
	}
	cerr << "sequencing graph loaded" << endl;
}

static bool range(int a, int n) { return 0 <= a && a < n; }

void DMFB::loadModuleLibrary()
{
	using namespace Global;
	ifstream is("./input/ModuleLibrary.txt");
	assert(is.is_open());
    least_time.resize(nDroplets, 0);
	// for (int i = 0; i < this->nDroplets; i++) {
	// 	leastTime[i] = 0;
	// }
	is >> this->nSinks;
	for (int i = 0; i < this->nDroplets; i++) 
		if (node[i]->fa == nullptr) 
			::dfs_least(node[i], this->least_time, this->droplet_data);
	
	cerr << "module library loaded" << endl;
}

void DMFB::loadDesignObejective()
{
	ifstream is("./input/DesignObjective.txt");
	assert(is.is_open());
	is >> this->rows >> this->columns;
	this->detector_record = new int*[this->rows];
	for (int i = 0; i < this->rows; i++) {
		this->detector_record[i] = new int[this->columns];
	}
	grid = new Grid(this->rows, this->columns);
	grid->build();	
	assert(this->nTypes <= grid->area());
	for (int i = 0; i < 4; i++) {
		this->boundary_record[i] = new int[grid->boundarySize[i]];
	}
	cerr << "design objective loaded" << endl;
}

static const State* ret;
static int* curBoundary[4];
static int** curDetector;
static int stepLowerBound, stepUpperBound;
static int retStep;
static int target;
static int curDispenserCount, curSinkCount, curDetectorCount;


bool DMFB::dfs(const State* currentState)
{
	bool flag = false;
	if (currentState->isEndState()) {
        flag = true;
        for (int k = 0; k < 4; k++) {
            for (int i = 0; i < grid->boundarySize[k]; i++) {
                this->boundary_record[k][i] = curBoundary[k][i];
            }
        }
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                this->detector_record[i][j] = 
                    curDetector[i][j];
            }
        }

        if (ret != nullptr) {
            ret->clean();
        }
        assert(system("mkdir -p output") == 0);
        char st[100];
        sprintf(st, "output/%d.out", currentState->step);
        ofstream os(st);
        this->printPlace(os);
        ret = currentState;
        ret->printSolution(os);
        os << "time: " << (clock() - Global::start_time) / 1e6 << "s" << endl;
        cerr << "solution of " << ret->step;
        if (ret->step == 1) {
            cerr << " step";
        } else {
            cerr << " steps";
        }
        cerr << " found" << endl;
        cerr << "time: " << (clock() - Global::start_time) / 1e6 << "s" << endl;
        target = ret->step - 1;
    } else {
		if (currentState->step + currentState->estimationTime() <= stepUpperBound) {
			vector<const State*> successors = currentState->getSuccessors();
			sort(successors.begin(), successors.end());
			for (auto successor: successors) {
				if (dfs(successor)) {
					flag = true;
					break;
				}
			}
		}
	}
	if (!flag) delete currentState;
	return flag;
}

void DMFB::placeDetector(int detectorCount)
{
	static int cnt = 0;
	if (detectorCount == this->nTypes) {
		// cerr << cnt++ << endl;
		if (!placeState->addDetector(detectorCount, this->detectors))
			return ;
		for (stepUpperBound = stepLowerBound; stepUpperBound <= target; stepUpperBound++) {
			hashSet.clear();
            // cerr << stepUpperBound << endl;
			State* init = new State;
			hashSet.insert(init->hash());
			if (dfs(init)) {
				assert(ret != nullptr);
				break;
			}
		}
	} else {
		for (int i = 0; i < grid->getRows(); i++) {
			for (int j = 0; j < grid->getColumns(); j++) {
				Point position = Point(i, j);
				Detector* detector = new Detector(detectorCount, position);
				this->detectors[detectorCount] = detector;
				if (grid->placeDetector(detector, position)) {
					// detectorPosition[detectorCount] = position;
					curDetector[i][j] = detectorCount;
					this->placeDetector(detectorCount + 1);
					curDetector[i][j] = -1;
					grid->removeDetector(position);
				}
				delete detector;
				if (target < stepLowerBound) return;
			}
		}
	}
}

void DMFB::placeSink(int sinkCount)
{
	if (sinkCount == this->nSinks) {
		if (placeState->addSink(sinkCount, sinks)) {
			placeState->clearDetector();
			this->placeDetector(0);
		}
	} else {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < grid->boundarySize[k]; i++) {
				if (curBoundary[k][i] == -1) {
					Point position = grid->boundaryPosition(i, k);
					Sink* sink = new Sink(position);
					sinks[sinkCount] = sink;
					if (grid->placeSink(sink, position)) {
						curBoundary[k][i] = this->nTypes;
						this->placeSink(sinkCount + 1);
						grid->removeSink(position);
						curBoundary[k][i] = -1;
					}
					delete sink;
					if (target < stepLowerBound) return;
				}
			}
		}
	}
}


void DMFB::placeDispenser(int dispenserCount)
{
	if (dispenserCount == this->nDispensers) {
        // cerr << "test" << endl;
		for (int i = 0; i < this->nDispensers; i++) {
			assert(grid->inside(dispensers[i]->getPosition()));
		}
		if (placeState->addDispenser(dispenserCount, dispensers)) {
			placeState->clearSink();
	    	this->placeSink(0);
		}
	} else {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < grid->boundarySize[k]; i++) {
				if (curBoundary[k][i] == -1) {
					Point position = grid->boundaryPosition(i, k);
					assert(grid->inside(position));
					dispensers[dispenserCount]->setPosition(position);
					curBoundary[k][i] = dispenserCount;
					bool flag = true;
					if (flag) this->placeDispenser(dispenserCount + 1);
					curBoundary[k][i] = -1;
					if (target < stepLowerBound) return;
				}
			}
		}
	}
}

void DMFB::solve()
{
	target = 100;
	curDetector = new int*[grid->getRows()];
	for (int i = 0; i < grid->getRows(); i++) {
		curDetector[i] = new int[grid->getColumns()];
		for (int j = 0; j < grid->getColumns(); j++) {
			curDetector[i][j] = -1;
		}
	}
	for (int i = 0; i < 4; i++) {
		curBoundary[i] = new int[grid->boundarySize[i]];
		for (int j = 0; j < grid->boundarySize[i]; j++) {
			curBoundary[i][j] = -1;
		}
	}
	assert(system("rm -f output/*") == 0);
	// sink = new Sink*[20];
    sinks.resize(20);
	// pdetector = new Detector*[20];
    detectors.resize(20);
	placeState = new PlaceState;
	placeState->set(grid->getRows(), grid->getColumns());
	placeState->clearDispenser();
	ret = nullptr;
    // cerr << "test" << endl;
	this->placeDispenser(0);
	for (int i = 0; i < 4; i++) {
		delete []curBoundary[i];
	}
	for (int i = 0; i < this->rows; i++) {
		delete []curDetector[i];
	}
	delete []curDetector;
	// delete []sink;
	// delete []pdetector;
    
	delete placeState;

}

void DMFB::print(ostream& os, int x)
{
	if (x == -1) {
		os << "N ";
	} else if (x == this->nTypes) {
		os << "S ";
	} else {
		assert(0 <= x && x < this->nTypes && x <= 9);
		os << "D" << type[x];
	}
}

void DMFB::printPlace(ostream& os)
{
	os << "placement: " << endl;
	os << "  |";
	for (int j = 0; j < this->columns; j++) {
		this->print(os, this->boundary_record[(int)Grid::OuterPos::UP][j]);
		os << "|";
	}
	os << endl;
	for (int i = 0; i < this->rows; i++) {
		this->print(os, this->boundary_record[(int)Grid::OuterPos::LEFT][i]);
		os << "|";
		for (int j = 0; j < this->columns; j++) {
			this->print(os, this->detector_record[i][j]);
			os<<"|";
		}
		this->print(os, this->boundary_record[(int)Grid::OuterPos::RIGHT][i]);
		os << endl;
	}
	os << "  |";
	for (int j = 0; j < this->columns; j++) {
		this->print(os, this->boundary_record[(int)Grid::OuterPos::DOWN][j]);
		os << "|";
	}
	os << endl;
}

#include <stack>

static vector<const State*> get_route(const State* start, const State* end)
{
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

//free states in queue that not in route
static void clear(vector<const State*> queue, vector<const State*> route)
{
	auto it = route.begin();
	for (auto state: queue) {
		if (state == *it)
			it++;
		else
			delete state;
	}

	assert(it == route.end());
}

#include <unordered_set>

vector<const State*> DMFB::get_route(const State* state) const
{
	unordered_set<State> state_set;
	if (state->isEndState())
		return vector<const State*>(1, state);	//already end state

	vector<const State*> queue;
 	queue.push_back(state);
	int head = 0, tail = 1;
	while (head < tail) {
		auto cur = queue[head++];
		for (auto suc: cur->getSuccessors()) {
			if (suc->isEndState()) {
				auto route = ::get_route(state, suc);
				::clear(queue, route);
				return route;
			} else if (!state_set.count(*suc)) {
				state_set.insert(*suc);
				queue.push_back(suc);
			}
		}
	}

	return vector<const State*>();	//	no solution
}

DMFB *DMFBsolver;