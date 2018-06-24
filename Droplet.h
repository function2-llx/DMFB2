#ifndef DROPLET_H
#define DROPLET_H

#include <iostream>
#include "Mixer.h"
#include "Direction.h"
#include "Dispenser.h"
#include "Dispensation.h"
#include "Detection.h"

typedef unsigned long long ULL;

class Droplet {
private:
    int identifier;
    int type;
    Point position;
    bool detecting;
    int remainingDetectingTime;

//    Droplet(const Droplet&);
    Droplet& operator = (const Droplet& droplet);
public:
    Droplet(Dispenser* dispenser, Dispensation* dispensation);
    Droplet(Droplet* precursor, Direction direction);
    Droplet(Mixer* mixer);
    ~Droplet();

    ULL hash();
    void startDetection();
    bool underDetection();
    bool detected() const;
    int getIdentifier();
    int getType();
    int getRemainingDetectinTime();
    Point getPosition();
    int estimatedTime();
    
    friend std::ostream& operator << (std::ostream&, const Droplet&);
};

#endif  //DROPLET_H