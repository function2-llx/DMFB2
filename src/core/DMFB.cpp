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
#include "useless/Hash.h"
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

int DMFB::getDropletNumber() const
{
	return this->nDroplets;
}

Node** node;

DMFB::DMFB()
{
	this->typeMap.clear();
	this->nTypes = 0;
	this->nDispensers = 0;
}

DMFB::~DMFB()
{
	for (int i = 0; i < 4; i++) {
		delete []this->boundary[i];
	}
	for (int i = 0; i < this->rows; i++) {
		delete []this->detector[i];
	}
	delete []this->detector;
}

int* leastTime;

static void dfsLeast(Node* cur)
{
	if (cur == nullptr) {
		return;
	}
	if (cur->fa != nullptr) {
		leastTime[cur->identifier] += leastTime[cur->fa->identifier] + dropletData[cur->fa->identifier].mixingTime + dropletData[cur->fa->identifier].detectingTime;
	}
	for (int i = 0; i < 2; i++) {
		dfsLeast(cur->ch[i]);
	}
}

vector<int> type;

void DMFB::loadSequencingGraph()
{
	using namespace Global;

	ifstream is("./input/SequencingGraph.txt");
	assert(is.is_open());

	is >> this->nDroplets;
	leastTime = new int[nDroplets];
	node = new Node*[this->nDroplets];
	toBeMixed = new bool[this->nDroplets];
	toBeDispensed = new bool[this->nDroplets];
	mixingResult = new int*[this->nDroplets];
	dropletData = new DropletData[this->nDroplets];

	for (int i = 0; i < this->nDroplets; i++) {
		mixingResult[i] = new int[this->nDroplets];
		node[i] = new Node();
		for (int j = 0; j < this->nDroplets; j++) {
			mixingResult[i][j] = -1;
		}
	}
	for (int i = 0; i < this->nDroplets; i++) {
		node[i]->identifier = i;
		dropletData[i].identifier = i;
		is >> node[i]->type;
		int fatherIndentifier;
		is >> fatherIndentifier;
		if (fatherIndentifier != 0) {
			fatherIndentifier--;
			node[i]->fa = node[fatherIndentifier];
			node[fatherIndentifier]->insertChild(node[i]);
		}
		is >> dropletData[i].mixingTime >> dropletData[i].detectingTime;
	}
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->fa == nullptr) {
			toBeMixed[i] = false;
		} else {
			toBeMixed[i] = true;
		}
	}
	for (int i = 0 ; i < this->nDroplets; i++) {
		if (node[i]->ch[0] == nullptr) {
			assert(dropletData[i].mixingTime  == 0);
			toBeDispensed[i] = true;
			assert(node[i]->ch[1] == nullptr);
			if (!this->typeMap.count(node[i]->type)) {
				this->typeMap[node[i]->type] = this->nDispensers++;
				type.push_back(node[i]->type);
			}
			node[i]->type = this->typeMap[node[i]->type];
		} else {
			toBeDispensed[i] = false;
		}
	}
	dispenser = new Dispenser*[this->nDispensers];
	for (int i = 0; i < this->nDispensers; i++) {
		dispenser[i] = new Dispenser(i);
	}
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
		dropletData[i].type = node[i]->type;
	}
	detectorPosition = new Point[this->nTypes];
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
		if (node[i]->fa == nullptr) {
			toBeMixed[i] = false;
		} else {
			toBeMixed[i] = true;
		}
		if (node[i]->ch[0] == nullptr) {
			assert(node[i]->ch[0] == nullptr);
		} else {
			Node *ch1 = node[i]->ch[0], *ch2 = node[i]->ch[1];
			assert(ch2 != nullptr);
			mixingResult[ch1->identifier][ch2->identifier] = mixingResult[ch2->identifier][ch1->identifier] = i;
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

static bool range(int a, int n)
{
	return 0 <= a && a < n;
}

void DMFB::loadModuleLibrary()
{
	using namespace Global;
	ifstream is("./input/ModuleLibrary.txt");
	assert(is.is_open());
	for (int i = 0; i < this->nDroplets; i++) {
		leastTime[i] = 0;
	}
	is >> this->nSinks;
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->fa == nullptr) {
			dfsLeast(node[i]);
		}
	}
	cerr << "module library loaded" << endl;
}

void DMFB::loadDesignObejective()
{
	ifstream is("./input/DesignObjective.txt");
	assert(is.is_open());
	is >> this->rows >> this->columns;
	this->detector = new int*[this->rows];
	for (int i = 0; i < this->rows; i++) {
		this->detector[i] = new int[this->columns];
	}
	grid = new Grid(this->rows, this->columns);
	grid->build();	
	assert(this->nTypes <= grid->area());
	for (int i = 0; i < 4; i++) {
		this->boundary[i] = new int[grid->boundarySize[i]];
	}
	cerr << "design objective loaded" << endl;
}

const State* ret;
int* curBoundary[4];
int** curDetector;
int stepLowerBound, stepUpperBound;
int retStep;
int target;
int curDispenserCount, curSinkCount, curDetectorCount;


bool DMFB::dfs(const State* currentState)
{
	bool flag = false;
	if (currentState->isEndState()) {
			flag = true;
			for (int k = 0; k < 4; k++) {
				for (int i = 0; i < grid->boundarySize[k]; i++) {
					this->boundary[k][i] = curBoundary[k][i];
				}
			}
			for (int i = 0; i < this->rows; i++) {
				for (int j = 0; j < this->columns; j++) {
					this->detector[i][j] = curDetector[i][j];
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
			os << "time: " << (clock() - Global::start_time/1e6) << "s" << endl;
			cerr << "solution of " << ret->step;
			if (ret->step == 1) {
				cerr << " step";
			} else {
				cerr << " steps";
			}
			cerr << " found" << endl;
			cerr << "time: " << (clock() - Global::start_time)/1e6 << "s" << endl;
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
	if (detectorCount == this->nTypes) {
		if (!placeState->addDetector(detectorCount, pdetector))
			return ;
		for (stepUpperBound = stepLowerBound; stepUpperBound <= target; stepUpperBound++) {
			hashSet.clear();
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
				::pdetector[detectorCount] = detector;
				if (grid->placeDetector(detector, position)) {
					detectorPosition[detectorCount] = position;
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
		if (placeState->addSink(sinkCount, sink)) {
			placeState->clearDetector();
			this->placeDetector(0);
		}
	} else {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < grid->boundarySize[k]; i++) {
				if (curBoundary[k][i] == -1) {
					Point position = grid->boundaryPosition(i, k);
					Sink* sink = new Sink(position);
					::sink[sinkCount] = sink;
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
		for (int i = 0; i < this->nDispensers; i++) {
			assert(grid->inside(dispenser[i]->getPosition()));
		}
		if (placeState->addDispenser(dispenserCount, dispenser)) {
			placeState->clearSink();
		this->placeSink(0);
		}
	} else {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < grid->boundarySize[k]; i++) {
				if (curBoundary[k][i] == -1) {
					Point position = grid->boundaryPosition(i, k);
					assert(grid->inside(position));
					dispenser[dispenserCount]->setPosition(position);
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
	sink = new Sink*[20];
	pdetector = new Detector*[20];
	placeState = new PlaceState;
	placeState->set(grid->getRows(), grid->getColumns());
	placeState->clearDispenser();
	ret = nullptr;
	this->placeDispenser(0);
	for (int i = 0; i < 4; i++) {
		delete []curBoundary[i];
	}
	for (int i = 0; i < this->rows; i++) {
		delete []curDetector[i];
	}
	delete []curDetector;
	delete []sink;
	delete []pdetector;
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
		this->print(os, this->boundary[1][j]);
		os << "|";
	}
	os << endl;
	for (int i = 0; i < this->rows; i++) {
		this->print(os, this->boundary[0][i]);
		os << "|";
		for (int j = 0; j < this->columns; j++) {
			this->print(os, this->detector[i][j]);
			os<<"|";
		}
		this->print(os, this->boundary[2][i]);
		os << endl;
	}
	os << "  |";
	for (int j = 0; j < this->columns; j++) {
		this->print(os, this->boundary[3][j]);
		os << "|";
	}
	os << endl;
}

vector<State*> DMFB::get_route(const State* state) const
{
	
}

DMFB *DMFBsolver;