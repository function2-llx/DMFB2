#pragma once

#include <vector>
#include "Washer.h"

typedef unsigned long long ULL;

class WashState {
private:
	std::vector<bool> *completed;
	std::vector<const Washer*> washers;
	int step;

	void dfs(unsigned int number) const;
public:

	const WashState* decision;
	std::vector<const WashState*> getSuccessors() const;
	bool check() const;
	ULL hash() const;
};