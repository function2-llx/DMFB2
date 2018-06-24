#ifndef CMP_H
#define CMP_H

#include "Dispensation.h"
#include "Droplet.h"
#include "MixingOperation.h"
#include "Mixer.h"

bool cmpDispensation(Dispensation* a, Dispensation* b);
bool cmpDroplet(Droplet* a, Droplet* b);
bool cmpMixingOperation(MixingOperation* a, MixingOperation* b);
bool cmpMixer(Mixer* a, Mixer* b);

#endif