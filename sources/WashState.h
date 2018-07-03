#pragma once

#include <vector>
#include "Washer.h"

typedef unsigned long long ULL;

class WashState {
private:
	bool *completed;
	std::vector<const Washer*> washers;
	int step;

	void dfs(unsigned int number) const;
	void pushWasher(const Washer& washer, unsigned int number) const;
	void addWasher(const Washer* washer);
public:
	const WashState* decision;

	WashState(Point position);
	WashState(const WashState* washState);
	~WashState();
	std::vector<const WashState*> getSuccessors() const;

	bool check() const;
	bool isEndState() const;
	ULL hash() const;
};