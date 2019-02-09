#pragma once

#include "Point.h"
#include "Direction.h"

typedef unsigned long long ULL;

class Washer {
	friend std::ostream& operator << (std::ostream&, const Washer&);
private:
	Point position;
public:
	Washer(const Washer* precursor, Direction direction);
	Washer(Point position);

	Point getPosition() const;
	ULL hash() const;
};