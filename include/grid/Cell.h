#ifndef CELL_H
#define CELL_H

#include <vector>
#include "math_models/Point.h"
#include "entities/Detector.h"
#include "entities/Sink.h"

class Cell {
  private:
    Point position;
    Detector* detector;
    Sink* sink;

    bool available;
  public:
    Cell();
    
    void setPosition(Point position);

    void setDetector(Detector* detector);
    bool existDetector();
    Detector* getDetector();
    void removeDetector();

    void setSink(Sink* sink);
    bool existSink();
    Sink* getSink();
    void removeSink();

    bool is_available() const { return this->available; }
    bool set_available(bool available) { this->available = available; }
};

#endif  //CELL_H