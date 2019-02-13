#include <cassert>
#include <stddef.h>
#include "grid/Cell.h"
#include "math_models/Point.h"
#include "grid/Grid.h"

Cell::Cell() : position(0, 0), detector(nullptr), sink(nullptr) {}

bool Cell::existDetector() { return this->detector != nullptr; }

bool Cell::existSink() { return this->sink != nullptr; }

void Cell::setPosition(Point position)
{
    assert(grid->inside(position)); 
    this->position = position;
}


void Cell::setDetector(Detector* detector)
{
    assert(this->detector == nullptr);
    this->detector = detector;
}

Detector* Cell::getDetector()
{
    return this->detector;
}

void Cell::removeDetector()
{
    assert(this->detector != nullptr);
    this->detector = nullptr;
}

void Cell::setSink(Sink* sink)
{
    assert(this->sink == nullptr);
    this->sink = sink;
}

Sink* Cell::getSink() { return this->sink; }

void Cell::removeSink()
{
    assert(this->sink != nullptr);
    this->sink = nullptr;
}