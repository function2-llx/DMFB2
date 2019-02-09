#include "Cmp.h"
#include "Droplet.h"
#include "Detector.h"

bool cmpDroplet(const Droplet* a, const Droplet* b)
{
	return a->getIdentifier() < b->getIdentifier();
}

bool cmpDetector(Detector* a, Detector* b)
{
	return a->getType() < b->getType();
}