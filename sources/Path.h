#pragma once

#include <vector>

struct Path {
	std::vector<int> path;
	void insert(int x);
	Path();
};