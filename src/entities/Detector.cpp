#include "math_models/Point.h"
#include "entities/Detector.h"
#include "grid/Grid.h"

Detector::Detector(int type, Point position) : type(type), position(position)
{
    
}

int Detector::getType()
{
    return this->type;
}

Point Detector::getPosition()
{
    return this->position;
}

ULL Detector::hash()
{
    return this->type * grid->area() + grid->getPointIdentifier(this->position);
}

// Point* detectorPosition;
// Detector** pdetector;