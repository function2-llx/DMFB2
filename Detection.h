#ifndef DETECTION_H
#define DETECTION_H

#include <istream>

struct Detection {
    int duration;
    
    friend std::istream& operator >> (std::istream&, Detection&);
};

extern Detection** detection;

#endif  //DETECTION_H