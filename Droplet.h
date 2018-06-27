#ifndef DROPLET_H
#define DROPLET_H

#include <iostream>
#include "Direction.h"
#include "Dispenser.h"
#include "Detection.h"

typedef unsigned long long ULL;

class Droplet {
private:
    int identifier;
    int type;
    Point position;

    bool mixing;
    int remainingMixingTime;    

    bool detecting;
    int remainingDetectingTime;

    Droplet& operator = (const Droplet& droplet);
public:
    Droplet(Droplet* precursor, Direction direction);
    Droplet(Droplet* droplet1, Droplet* droplet2);
    ~Droplet();

    ULL hash();

    bool underMixing();

    void startDetection();
    bool underDetection();
    bool detected() const;

    int getIdentifier();
    int getType();
    Point getPosition();

    int estimatedTime();
    
    friend std::ostream& operator << (std::ostream&, const Droplet&);
};

#endif  //DROPLET_H