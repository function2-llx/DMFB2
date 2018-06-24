#include "Point.h"
#include "Detector.h"
#include "Grid.h"

Detector::Detector(int type, Point position) : type(type), position(position)
{
    
}

int Detector::getType()
{
    return this->type;
}

Point Detector::getPositon()
{
    return this->position;
}

ULL Detector::hash()
{
    return this->type * grid->area() + grid->getPointIdentifier(this->position);
}

Point* detectorPosition;