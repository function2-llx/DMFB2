#include "Cmp.h"
#include "Droplet.h"
#include "Detector.h"

bool cmpDispensation(Dispensation* a, Dispensation* b)
{
	return a->identifier < b->identifier;
}

bool cmpDroplet(Droplet* a, Droplet* b)
{
	return a->getIdentifier() < b->getIdentifier();
}

bool cmpMixingOperation(MixingOperation* a, MixingOperation* b)
{
	return a->mixerIdentifer < b->mixerIdentifer;
}

bool cmpMixer(Mixer* a, Mixer* b)
{
	return a->getIdentifier() < b->getIdentifier();
}

bool cmpDetector(Detector* a, Detector* b)
{
	return a->getType() < b->getType();
}