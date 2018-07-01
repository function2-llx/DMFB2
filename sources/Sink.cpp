#include "Sink.h"

Sink::Sink(Point positon)
{
	this->position = position;
}

Point Sink::getPosition() {
	return this->position;
}

Sink** sink;