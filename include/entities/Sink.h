#ifndef SINK_H
#define SINK_H

#include "math_models/Point.h"

class Sink {
private:
    Point position;
    Sink(const Sink& sink);
    Sink& operator = (const Sink& sink);
public:
    explicit Sink(const Point& position);
    Point getPosition();
};

extern Sink** sink;

#endif  //SINK_H