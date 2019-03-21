#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include "math_models/Point.h"

typedef unsigned long long ULL;

class Detector {
  private:
    // int type;
    Point position;

    Detector(const Detector& detector) = delete;
    Detector& operator = (const Detector& detector) = delete;
  public:
    // Detector(int type, Point position);
    explicit Detector() {}

    // int get_type() const { return type; }
    Point get_pos() const { return position; }
    void set_pos(const Point& pos) { position = pos; }
    
    ULL hash();
};

// extern Point* detectorPosition;
// extern Detector** pdetector;

#endif  //DETECTOR_H