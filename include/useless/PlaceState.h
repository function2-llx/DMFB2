#ifndef PLACESTATE_H
#define PLACESTATE_H

#include "entities/Dispenser.h"
#include "entities/Detector.h"
#include "entities/Sink.h"
#include "math_models/Point.h"

#include <vector>
#include <set>

class PlaceState {
  int r, c;
  std::set<std::pair<int, std::vector<Point>>> dispenserState;
  std::set<std::pair<int, std::vector<Point>>> sinkState;
  std::set<std::pair<int, std::vector<Point>>> detectorState;

  PlaceState(const PlaceState& placeState);
  PlaceState operator = (const PlaceState& placeState);
public:
  PlaceState() {}
  bool addDispenser(int dispenserCount, Dispenser** dispenser);
  bool addSink(int sinkCount, Sink** sink);
  bool addDetector(int detectorCount, Detector** detector);
  void clearDispenser();
  void clearSink();
  void clearDetector();
  void rotate(Point &point);
  void horizontal(Point &point);
  void vertical(Point &point);
  void set(int r_, int c_);
};

extern PlaceState* placeState;

#endif