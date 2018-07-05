#pragma once

#include <vector>
#include "Washer.h"

typedef unsigned long long ULL;

class WashState {
	friend std::ostream& operator << (std::ostream&, const WashState&);
private:
	bool *completed;
	std::vector<const Washer*> washers;

	void dfs(unsigned int number) const;
	void pushWasher(const Washer& washer, unsigned int number) const;
	void pushWasher(const Washer& washer, int type, int identifier) const;
	void dispense(int type, int identifier) const;
	void addWasher(const Washer* washer);
public:
	const WashState* decision;
	int step;

	WashState();
	WashState(const WashState* washState);
	~WashState();
	
	void clean() const;
	std::vector<const WashState*> getSuccessors() const;

	std::vector<const Washer*> getWashers() const;
	bool check() const;
	bool isEndState() const;
	ULL hash() const;

	void printRecursively(std::ostream&) const;
};