#ifndef SINK_H
#define SINK_H

#include "math_models/Point.h"

class Sink {
  private:
    Point position;
    Sink(const Sink& sink);
    Sink& operator = (const Sink& sink);
  public:
    // explicit Sink(const Point& position);
    explicit Sink() {}
    Point get_pos() const{ return this->position; }
    void set_pos(const Point& pos) { position = pos; }
};

#endif  //SINK_H