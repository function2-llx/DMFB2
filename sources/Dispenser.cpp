#include "Dispenser.h"
#include "Grid.h"
#include <vector>
#include <cassert>

using namespace std;

Dispenser::Dispenser(int type) : type(type) {}

int Dispenser::getType()
{
    return this->type;
}

void Dispenser::setPosition(Point position)
{
    assert(grid->inside(position));
    this->position = position;
}

Point Dispenser::getPosition()
{
    return this->position;
}

Dispenser* dispenser;