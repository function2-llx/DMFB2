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
    // Detector(int type, Point position);
    explicit Detector(int type) : type(type) {}

    int get_type() const { return type; }
    Point get_pos() const { return position; }
    void set_pos(const Point& pos) { position = pos; }
    
    ULL hash();
};

// extern Point* detectorPosition;
// extern Detector** pdetector;

#endif  //DETECTOR_H