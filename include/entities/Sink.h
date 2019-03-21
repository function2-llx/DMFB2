#ifndef SINK_H
#define SINK_H

#include "math_models/Point.h"

class Sink {
  private:
    int id;
    Point pos;
    Sink(const Sink& sink);
    Sink& operator = (const Sink& sink);
  public:
    explicit Sink(int id) : id(id) {}
    Point get_pos() const{ return this->pos; }
    void set_pos(const Point& pos) { this->pos = pos; }
    int get_id() const { return this->id; }
};

#endif  //SINK_H