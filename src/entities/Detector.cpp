#include "math_models/Point.h"
#include "entities/Detector.h"
#include "grid/Grid.h"


ULL Detector::hash() { return grid->getPointIdentifier(this->position); }

// Point* detectorPosition;
// Detector** pdetector;