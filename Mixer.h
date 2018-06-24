#ifndef MIXER_H
#define MIXER_H

#include "Point.h"
#include "MixingOperation.h"

typedef unsigned long long ULL;

class Mixer {
    friend std::ostream& operator << (std::ostream&, const Mixer&);
private:
    int identifier;
    int outputIdentifier, outputType;
    Point upperLeftCorner, lowerRightCorner;
    Point outputPosition;
    int remainingTime;
    Mixer& operator = (const Mixer& mixer);
public:
    Mixer(MixingOperation* mixingoperation, Point inputPosition, Point upperLeftCorner);
    Mixer(Mixer* precursor);
    ULL hash();
    int getIdentifier();
    bool inside(Point position);
    int getOutputIdentifier();
    int getOutputType();
    Point getOutputPosition();
    int getRemainingTime();
    bool overlap(Mixer* mixer);
    Point getUpperleftCorner();
    Point getLowerRightCorner();
    int estimatedTime();
};

#endif  //MIXER_H
