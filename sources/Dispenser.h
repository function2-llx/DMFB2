#ifndef DISPENSER_H
#define DISPENSER_H

#include <vector>
#include "Point.h"

typedef unsigned long long ULL;

class Dispenser {
private:
    int type;
    Point position;

    Dispenser(const Dispenser& dispenser);
    Dispenser operator = (const Dispenser& dispenser);
public:
    Dispenser(int type);
    int getType();
    void setPosition(Point position);
    Point getPosition();
};

//dispenser for each type
extern Dispenser** dispenser;

#endif  //DISPENSER_H