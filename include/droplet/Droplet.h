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
    int output_sink;
    Point position;

    bool dispensed;

    bool mixing;
    int remainingMixingTime;    

    bool detecting;
    int remainingDetectingTime;
    //call when a droplet first appears
    void setData(const DropletData& dropletData);

    Droplet& operator = (const Droplet& droplet);

    friend class State;
  public:
    Droplet(const DropletData&);    //  construct a droplet before dispensed
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

    void time_past(int);

    // int getIdentifier() const;
    int get_id() const { return this->identifier; }
    int getType() const;
    Point get_pos() const;

    int estimatedTime() const;
    bool isEndDroplet() const;

    // Droplet* get_moved_droplet(const Direction&) const;
    // static Droplet* merge(const Droplet*, const Droplet*);
    
    friend std::ostream& operator << (std::ostream&, const Droplet&);
    friend bool operator == (const Droplet&, const Droplet&);
    inline friend bool operator != (const Droplet& a, const Droplet& b) { return !(a == b); }
};

#endif  //DROPLET_H