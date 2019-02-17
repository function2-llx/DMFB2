#include "math_models/Point.h"
#include "entities/Detector.h"
#include "grid/Grid.h"


ULL Detector::hash() { return this->type * grid->area() + grid->getPointIdentifier(this->position); }

// Point* detectorPosition;
// Detector** pdetector;