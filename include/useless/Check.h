#ifndef CHECK_H
#define CHECK_H

#include "core/State.h"
#include "useless/WashState.h"

int check(State*, WashState*);// 0 = correct , 1 = invalid moving, 2 = fluidic constrains(droplet & droplet), 3 = fluidic constrains(droplet & washer)

#endif
