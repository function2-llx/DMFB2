#ifndef DISPENSER_H
#define DISPENSER_H

#include <vector>
#include "math_models/Point.h"

class Dispenser {
private:
    int type;
    Point position;

    Dispenser(const Dispenser& dispenser);
    Dispenser operator = (const Dispenser& dispenser);
public:
    Dispenser(int type);
    int get_type() const { return this->type; }
    void setPosition(Point position);
    Point getPosition();
};

//dispenser for each type
// extern Dispenser** dispenser;

#endif  //DISPENSER_H