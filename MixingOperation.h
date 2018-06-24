#ifndef MIXINGOPERATION_H
#define MIXINGOPERATION_H

#include <vector>
#include <istream>

struct MixingOperation {
    int rows, columns, duration;
    int inputIdentifier[2];
    int outputIdentifier, outputType;
    //the Identifier of the mixer it produces
    int mixerIdentifer;
    
    MixingOperation();
    bool match(int identifier1, int identifier2);
friend std::istream& operator >> (std::istream&, MixingOperation&);
friend std::ostream& operator << (std::ostream&, const MixingOperation&);
};

extern std::vector<MixingOperation*> allMixingOperations;
extern bool* toBeMixed;
extern bool** mixPair;
extern MixingOperation** mixingData;

#endif