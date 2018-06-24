#ifndef CMP_H
#define CMP_H

#include "Droplet.h"

bool cmpDispensation(Dispensation* a, Dispensation* b);
bool cmpDroplet(Droplet* a, Droplet* b);
bool cmpMixingOperation(MixingOperation* a, MixingOperation* b);
bool cmpMixer(Mixer* a, Mixer* b);

#endif