#include <cassert>
#include "Grid.h"
#include "WashState.h"
#include "WasherRouter.h"
#include "Direction.h"
#include "Hash.h"

using namespace std;

WashState::WashState()
{
	this->step = -1;
	this->decision = nullptr;
	int size = washerRouter->getWashes().size();
	this->completed = new bool[size];
	for (int i = 0; i < size; i++) {
		this->completed[i] = false;
	}
	this->washers.clear();
}

WashState::WashState(const WashState* precursor)
{
	this->step = precursor->step + 1;
	this->decision = precursor;
	int size = washerRouter->getWashes().size();
	this->completed = new bool[size];
	for (int i = 0; i < size; i++) {
		this->completed[i] = precursor->completed[i];
	}
	this->washers.clear();
}

WashState::~WashState()
{
	delete[] completed;
}

void WashState::addWasher(const Washer* washer)
{
	this->washers.push_back(washer);
	Point position = washer->getPosition();
	auto washes = washerRouter->getWashes();
	for (int i = 0; i < washes.size(); i++) {
		auto wash = washes[i];
		if (wash.t1 <= this->step && wash.position == position) {
			this->completed[i] = true;
			break;
		}
	}
}

bool WashState::check() const
{
	auto washes = washerRouter->getWashes();
	for (int i = 0; i < washes.size(); i++) {
		auto wash = washes[i];
		if (!this->completed[i] && wash.t2 == this->step) {
			return false;
		}
	}
	return true;
}

static vector<const WashState*> successors;
static vector<Washer> **content;
static bool **preRecord, **curRecord;

static bool canPush(int time, Point position)
{
	if (!grid->inside(position)) return false;
	if (preRecord[position.r][position.c] || curRecord[position.r][position.c]) return false;
	return washerRouter->canReach(time, position);
}

void WashState::pushWasher(const Washer& washer, unsigned int number) const
{
	Point position = washer.getPosition();
	content[position.r][position.c].push_back(washer);
	int record[3][3];
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (grid->inside(position + Direction(i, j))) {
				record[i + 1][j + 1] = curRecord[position.r + i][position.c + j];
			}
		}
	}
	this->dfs(number + 1);
	for (int i = -1; i <= 1; i++) {
		for (int j = -1 ;j <= 1; j++) {
			if (grid->inside(position + Direction(i, j))) {
				curRecord[position.r + i][position.c + j] = record[i + 1][j + 1];
			}
		}
	}
}

void WashState::dfs(unsigned int number) const
{
	if (number == this->washers.size()) {
		WashState *state = new WashState(this);
		for (int i = 0 ; i < grid->getRows(); i++) {
			for (int j = 0; j < grid->getColumns(); i++) {
				if (content[i][j].size() == 1) {
					state->addWasher(new Washer(content[i][j][0]));
				}
			}
		}
		if (state->check()) {
			auto hash = state->hash();
			if (!washHashSet.count(hash)) {
				washHashSet.insert(hash);
				successors.push_back(state);				
			}
		} else {
			delete state;
		}
	} else {
		auto washer = this->washers[number];
		Point position = washer->getPosition();
		for (int i = 0; i < 5; i++) {
			Point cur = position + direction[i];
			if (::canPush(this->step + 1, cur)) {
				this->pushWasher(Washer(washer, direction[i]), number + 1);
			}
		}
	}
}

vector<const WashState*> WashState::getSuccessors() const
{
	content = new vector<Washer>*[grid->getRows()];
	preRecord = new bool*[grid->getRows()];
	curRecord = new bool*[grid->getRows()];
	for (int i = 0; i < grid->getColumns(); i++) {
		content[i] = new vector<Washer>[grid->getColumns()];
		preRecord[i] = new bool[grid->getColumns()];
		curRecord[i] = new bool[grid->getColumns()];
		for (int j = 0; j <  grid->getColumns(); j++) {
			content[i][j].clear();
			preRecord[i][j] = false;
			curRecord[i][j] = false;
		}
	}
	for (auto washer: this->washers) {
		auto position = washer->getPosition();
		for (int i = -1; i <= 1; i++) {
			for (int j = -1; j <= 1; j++) {
				if (grid->inside(position + Direction(i, j))) {
					preRecord[position.r + i][position.c + j] = true;
				}
			}
		}
	}
	this->dfs(0);
	for (int i = 0; i < grid->getRows(); i++) {
		delete[] content[i];
		delete[] preRecord[i];
		delete[] curRecord[i];
	}
	delete[] content;
	delete[] preRecord;
	delete[] curRecord;
	return successors;
}

bool WashState::isEndState() const
{
	int size = washerRouter->getWashes().size();
	for (int i = 0; i < size; i++) {
		if (!this->completed[i]) {
			return false;
		}
	}
	return true;
}

ULL WashState::hash() const
{
	
}