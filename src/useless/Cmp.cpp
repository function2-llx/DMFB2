#include "useless/Cmp.h"
#include "droplet/Droplet.h"
#include "entities/Detector.h"

bool cmpDroplet(const Droplet* a, const Droplet* b)
{
	return a->getIdentifier() < b->getIdentifier();
}

bool cmpDetector(Detector* a, Detector* b)
{
	return a->getType() < b->getType();
}