#include "Sink.h"

#include <iostream>
#include <cassert>

using namespace std;

Sink::Sink(const Point& position)
{
	this->position = position;
}

Point Sink::getPosition() {
	return this->position;
}

Sink** sink;