#include <cassert>
#include "Grid.h"
#include "WashState.h"
#include "WasherRouter.h"
#include "Direction.h"
#include "Hash.h"

using namespace std;

static int tt;

ostream& operator << (ostream& os, const WashState& state)
{
	bool **type;
	type = new bool*[grid->getRows()];
	for (int i = 0; i < grid->getRows(); i++) {
		type[i] = new bool[grid->getColumns()];
		for (int j = 0; j < grid->getColumns(); j++) {
			type[i][j] = false;
		}
	}
	auto washers = state.getWashers();
	for (auto washer: washers) {
		Point position = washer->getPosition();
		type[position.r][position.c] = true;
	}
	for (int i = 0; i < grid->getRows(); i++) {
		for (int j = 0 ; j < grid->getColumns(); j++) {
			if (type[i][j]) os << "W ";
			else os << "N ";
		}
		os << endl;
	}
	return os;
}

WashState::WashState()
{
	this->step = 0;
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
	for (auto washer: this->washers) {
		delete washer;
	}
}

void WashState::clean() const
{
	if (this->decision != nullptr) {
		this->decision->clean();
	}
	delete this;
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
	if (preRecord[position.r][position.c] || curRecord[position.r][position.c]) {
		return false;
	}
	return washerRouter->canReach(time, position);
}

void WashState::pushWasher(const Washer& washer, int type, int identifier) const
{
	Point position = washer.getPosition();
	int record[3][3];
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			Point cur = position + Direction(i, j);
			if (grid->inside(cur)) {
				record[i + 1][j + 1] = curRecord[cur.r][cur.c];
				curRecord[cur.r][cur.c] = true;
			}
		}
	}
	content[position.r][position.c].push_back(washer);
	this->dispense(type, identifier + 1);
	content[position.r][position.c].pop_back();
	for (int i = -1; i <= 1; i++) {
		for (int j = -1 ;j <= 1; j++) {
			if (grid->inside(position + Direction(i, j))) {
				curRecord[position.r + i][position.c + j] = record[i + 1][j + 1];
			}
		}
	}
}

void WashState::dispense(int type, int identifier) const
{
	if (type == 4) {
		WashState *state = new WashState(this);
		assert(state->step == this->step + 1);
		for (int i = 0 ; i < grid->getRows(); i++) {
			for (int j = 0; j < grid->getColumns(); j++) {
				if (content[i][j].size() == 1) {
					state->addWasher(new Washer(content[i][j][0]));
				}
			}
		}
		assert(state->step == this->step + 1);
		if (state->check()) {
			auto hash = state->hash();
			assert(state->step == this->step + 1);
			if (!washHashSet.count(hash)) {
				washHashSet.insert(hash);
				assert(state->step == this->step + 1);
				successors.push_back(state);
				assert(state->step == this->step + 1);
			} else {
				delete state;
			}
		} else {
			delete state;
		}
	} else {
		if (identifier == grid->boundarySize[type]) {
			this->dispense(type + 1, 0);
		} else {
			this->dispense(type, identifier + 1);
			if (::canPush(this->step + 1, grid->boundaryPosition(identifier, type))) {
				this->pushWasher(Washer(grid->boundaryPosition(identifier, type)), type, identifier);
			}
		}
	}
}

void WashState::pushWasher(const Washer& washer, unsigned int number) const
{
	Point position = washer.getPosition();
	int record[3][3];
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			Point cur = position + Direction(i, j);
			if (grid->inside(cur)) {
				record[i + 1][j + 1] = curRecord[cur.r][cur.c];
				curRecord[cur.r][cur.c] = true;
			}
		}
	}
	content[position.r][position.c].push_back(washer);
	this->dfs(number + 1);
	content[position.r][position.c].pop_back();
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
		this->dispense(0, 0);
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

vector<const Washer*> WashState::getWashers() const
{
	return this->washers;
}

vector<const WashState*> WashState::getSuccessors() const
{
	tt = clock();
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
	successors.clear();
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
	const static ULL hashBase = 849169143145ull, shift = 4378196491ull;
	ULL ret = 0;
	int size = washerRouter->getWashes().size();
	for (int i = 0; i < size; i++) {
		ret = ret << 1 | this->completed[i];
	}
	for (auto washer: this->washers) {
		ret = washer->hash() + shift + hashBase * ret;
	}
	return ret;
}

void WashState::printRecursively(ostream& os) const
{
	if (this->decision != nullptr) {
		this->decision->printRecursively(os);
	}
	os << "step " << this->step << endl;
	os << *this << endl;
}