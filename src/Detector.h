#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>

typedef unsigned long long ULL;

class Detector {
private:
    int type;
    Point position;

    Detector(const Detector& detector);
    Detector& operator = (const Detector& detector);
public:
    Detector(int type, Point position);
    int getType();
    Point getPosition();
    ULL hash();
};

extern Point* detectorPosition;
extern Detector** pdetector;

#endif  //DETECTOR_H