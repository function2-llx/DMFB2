#ifndef DISPENSATION_H
#define DISPENSATION_H

#include <vector>
#include <istream>

typedef unsigned long long ULL;

struct Dispensation {
    int identifier, type;

    Dispensation(int identifier, int type);
    bool operator < (const Dispensation& dispensation);
    void print();
    int estimatedTime();

    friend std::ostream& operator << (std::ostream&, const Dispensation&);
};

extern std::vector<Dispensation*> allDispensations;

#endif  //DISPENSATION_H