#include <cassert>
#include "Washer.h"
#include "Grid.h"

Washer::Washer(Point position) : position(position)
{
	assert(grid->inside(this->position));
}

Washer::Washer(const Washer* precursor, Direction direction)
{
	this->position = precursor->position + direction;
	assert(grid->inside(this->position));
}

Point Washer::getPosition() const
{
	return this->position;
}