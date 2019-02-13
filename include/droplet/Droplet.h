#ifndef DROPLET_H
#define DROPLET_H

#include <iostream>
#include "math_models/Direction.h"
#include "entities/Dispenser.h"
#include "droplet/DropletData.h"

typedef unsigned long long ULL;

class Droplet {
  private:
    int identifier;
    int type;
    Point position;

    bool dispensed;

    bool mixing;
    int remainingMixingTime;    

    bool detecting;
    int remainingDetectingTime;
    //call when a droplet first appears
    void setData(const DropletData& dropletData);

    Droplet& operator = (const Droplet& droplet);
  public:
    Droplet(const DropletData& dropletData);
    Droplet(const Droplet* precursor, const Direction& direction);
    Droplet(const Droplet& droplet1, const Droplet& droplet2);
    Droplet(const Droplet&);

    ULL hash() const;

    bool is_dispensed() const;

    bool underMixing() const;
    bool mixed() const;

    void startDetection();
    bool underDetection() const;
    bool detected() const;

    int getIdentifier() const;
    int getType() const;
    Point getPosition() const;

    int estimatedTime() const;
    bool isEndDroplet() const;
    
    friend std::ostream& operator << (std::ostream&, const Droplet&);
    friend bool operator == (const Droplet&, const Droplet&);
    inline friend bool operator != (const Droplet& a, const Droplet& b) { return !(a == b); }
};

#endif  //DROPLET_H