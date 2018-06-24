#ifndef CELL_H
#define CELL_H

#include <vector>
#include "Point.h"
#include "Detector.h"
#include "Sink.h"

class Cell {
private:
    //actual neighbors besides cell itself
    Point position;
    Detector* detector;
    Sink* sink;
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
};

#endif  //CELL_H