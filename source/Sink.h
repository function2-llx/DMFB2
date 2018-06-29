#ifndef SINK_H
#define SINK_H

#include "Point.h"

class Sink {
private:
    Point position;
    Sink(const Sink& sink);
    Sink& operator = (const Sink& sink);
public:
    Sink(Point position);
};

#endif  //SINK_H