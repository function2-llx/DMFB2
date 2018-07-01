#pragma once

#include <vector>
#include "Washer.h"

class WashState {
private:
	bool* completed;
	std::vector<const Washer*> washers;
public:
	const WashState* decision;
};