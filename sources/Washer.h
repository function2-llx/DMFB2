#pragma once

#include "Point.h"
#include "Direction.h"

class Washer {
private:
	Point position;
public:
	Washer(const Washer* precursor, Direction direction);
	Washer(Point position);
	
};