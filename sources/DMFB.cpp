#include <set>
#include <map>
#include <queue>
#include <cassert>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "DMFB.h"
#include "State.h"
#include "Grid.h"
#include "Dispenser.h"
#include "Hash.h"
#include "Tools.h"

using namespace std;

struct Node {
	int identifier, type;
	Node* fa;
	Node* ch[2];
	Node() {
		fa = nullptr;
		ch[0] = ch[1] = nullptr;
	}
	void insertChild(Node* child)
	{
		if (this->ch[0] == nullptr) {
			this->ch[0] = child;
		} else {
			this->ch[1] = child;
		}
	}
};

Node** node;
int nDropletsOutsideClass;

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

int nMixers;
int* leastTime;

void dfsLeast(Node* cur)
{
	if (cur == nullptr) {
		return;
	}
	if (cur->fa != nullptr) {
		leastTime[cur->identifier] += leastTime[cur->fa->identifier] + dropletData[cur->fa->identifier].detectingTime;
	}
	for (int i = 0; i < 2; i++) {
		dfsLeast(cur->ch[i]);
	}
}

void DMFB::loadSequencingGraph()
{
	ifstream is("./input/SequencingGraph.txt");
	assert(is.is_open());
	is >> this->nDroplets;
	leastTime = new int[nDroplets];
	nDropletsOutsideClass = nDroplets;
	node = new Node*[this->nDroplets];
	toBeMixed = new bool[this->nDroplets];
	for (int i = 0; i < nDroplets; i++) {
		node[i] = new Node();
	}
	for (int identifier = 0; identifier < this->nDroplets; identifier++) {
		node[identifier]->identifier = identifier;
		is >> node[identifier]->type;
		int fatherIndentifier;
		is >> fatherIndentifier;
		if (fatherIndentifier != 0) {
			fatherIndentifier--;
			node[identifier]->fa = node[fatherIndentifier];
			node[fatherIndentifier]->insertChild(node[identifier]);
		}
	}
	for (int i = 0 ; i < this->nDroplets; i++) {
		if (node[i]->ch[0] == nullptr) {
			assert(node[i]->ch[1] == nullptr);
			if (!this->typeMap.count(node[i]->type)) {
				this->typeMap[node[i]->type] = this->nDispensers++;
			}
			node[i]->type = this->typeMap[node[i]->type];
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
			}
			node[i]->type = this->typeMap[node[i]->type];
		}
	}
	detectorPosition = new Point[this->nTypes];
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
	nMixers = 0;
	int cnt = 0;
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->fa == nullptr) {
			toBeMixed[i] = false;
		} else {
			toBeMixed[i] = true;
		}
		if (node[i]->ch[0] == nullptr) {
			assert(node[i]->ch[0] == nullptr);
			cnt++;
		} else {
			nMixers++;
			assert(node[i]->ch[1] != nullptr);
			data cur(node[i]->ch[0]->type, node[i]->ch[1]->type);
			if (mixType.count(cur)) {
				assert(node[i]->type == mixType[cur]);
			} else {
				mixType[cur] = node[i]->type;
			}
		}
	}
}

bool range(int a, int n)
{
	return 0 <= a && a < n;
}

void DMFB::loadModuleLibrary()
{
	ifstream is("./input/ModuleLibrary.txt");
	assert(is.is_open());
	is >> this->nMixingOperations;
	assert(nMixers == this->nMixingOperations);
	for (int i = 0; i < this->nDroplets; i++) {
		leastTime[i] = 0;
	}
	mixPair = new int*[this->nDroplets];
	is >> this->nSinks;
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->fa == nullptr) {
			dfsLeast(node[i]);
		}
	}
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
}

State* ret;
int* curBoundary[4];
int** curDetector;
int stepLowerBound, stepUpperBound;
int retStep;
int target;

bool DMFB::dfs(State* currentState)
{
	if (currentState->isEndState()) {
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
		char st[100];
		sprintf(st, "output/%d.out", currentState->step);
		ofstream os(st);
		this->printPlace(os);
		ret = currentState;
		ret->printSolution(os);
		cerr << "solution of " << ret->step;
		if (ret->step == 1) {
			cerr << " step";
		} else {
			cerr << " steps";
		}
		cerr << " found" << endl;
		target = ret->step - 1;
		return true;
	}
	bool flag = false;
	if (currentState->step + currentState->estimatedTime() <= stepUpperBound) {
		vector<State*> successors = currentState->getSuccessors();
		sort(successors.begin(), successors.end());
		for (auto successor: successors) {
			if (successor->isEndState()) {
				dfs(successor);
				flag = true;
				break;
			}
		}
		if (flag) {
			for (auto successor: successors) {
				if (successor != ret) {
					delete successor;
				}
			}
		} else {
			for (auto successor: successors) {
				if (!flag) {
					if (dfs(successor)) {
						flag= true;
					}
				} else {
					delete successor;
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
		for (stepUpperBound = stepLowerBound; stepUpperBound <= target; stepUpperBound++) {
			hashSet.clear();
			State* init = State::initialState();
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
		this->placeDetector(0);
	} else {
		for (int k = 0; k < 4; k++) {
			for (int i = 0; i < grid->boundarySize[k]; i++) {
				if (curBoundary[k][i] == -1) {
					Point position = grid->boundaryPosition(i, k);
					Sink* sink = new Sink(position);
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
		this->placeSink(0);
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
	target = 20;
	State* init = State::initialState();
	stepLowerBound = init->estimatedTime();
	delete init;
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
	system("rm -f output/*");
	ret = nullptr;
	this->placeDispenser(0);
	for (int i = 0; i < 4; i++) {
		delete []curBoundary[i];
	}
	for (int i = 0; i < this->rows; i++) {
		delete []curDetector[i];
	}
	delete []curDetector;
}

void DMFB::print(ostream& os, int x)
{
	if (x == -1) {
		os << "N ";
	} else if (x == this->nTypes) {
		os << "S ";
	} else {
		assert(0 <= x && x < this->nTypes && x <= 9);
		os << "D" << x;
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
		os<<"|";
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