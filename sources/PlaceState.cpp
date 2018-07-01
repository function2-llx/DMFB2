#include "PlaceState.h"
#include <algorithm>

void PlaceState::rotate(Point &point) {
  int x = point.r * 2 - r, y = point.c * 2 - c;
  point.r = (r - y) / 2, point.c = (c + x) / 2;
}

void PlaceState::horizontal(Point &point) {
  point.r = r - point.r - 1;
}

void PlaceState::vertical(Point &point) {
  point.c = c - point.c - 1;
}

bool PlaceState::addDispenser(int dispenserCount, Dispenser** dispenser) {
  std::vector<Point> vec;
  for (int i = 0; i < dispenserCount; ++i)
    vec.push_back(dispenser[i]->getPosition());
  std::sort(vec.begin(), vec.end());
  if (dispenserState.insert(std::make_pair(dispenserCount, vec)).second == false)
    return false;

  for (int dire = 0; dire < 4; ++dire) {
    std::sort(vec.begin(), vec.end());
    dispenserState.insert(std::make_pair(dispenserCount, vec));

    for (int i = 0; i < dispenserCount; ++i) horizontal(vec[i]);
    std::sort(vec.begin(), vec.end());
    dispenserState.insert(std::make_pair(dispenserCount, vec));

    for (int i = 0; i < dispenserCount; ++i) vertical(vec[i]);
    std::sort(vec.begin(), vec.end());
    dispenserState.insert(std::make_pair(dispenserCount, vec));
    
    for (int i = 0; i < dispenserCount; ++i) horizontal(vec[i]);
    std::sort(vec.begin(), vec.end());
    dispenserState.insert(std::make_pair(dispenserCount, vec));

    for (int i = 0; i < dispenserCount; ++i) vertical(vec[i]);
    for (int i = 0; i < dispenserCount; ++i) rotate(vec[i]);
  }
  return true;
}

bool PlaceState::addSink(int sinkCount, Sink** sink) {
  std::vector<Point> vec;
  for (int i = 0; i < sinkCount; ++i)
    vec.push_back(sink[i]->getPosition());
  std::sort(vec.begin(), vec.end());
  if (sinkState.insert(std::make_pair(sinkCount, vec)).second == false)
    return false;

  for (int dire = 0; dire < 4; ++dire) {
    std::sort(vec.begin(), vec.end());
    sinkState.insert(std::make_pair(sinkCount, vec));

    for (int i = 0; i < sinkCount; ++i) horizontal(vec[i]);
    std::sort(vec.begin(), vec.end());
    sinkState.insert(std::make_pair(sinkCount, vec));

    for (int i = 0; i < sinkCount; ++i) vertical(vec[i]);
    std::sort(vec.begin(), vec.end());
    sinkState.insert(std::make_pair(sinkCount, vec));
    
    for (int i = 0; i < sinkCount; ++i) horizontal(vec[i]);
    std::sort(vec.begin(), vec.end());
    sinkState.insert(std::make_pair(sinkCount, vec));

    for (int i = 0; i < sinkCount; ++i) vertical(vec[i]);
    for (int i = 0; i < sinkCount; ++i) rotate(vec[i]);
  }
  return true;
}

bool PlaceState::addDetector(int detectorCount, Detector** detector) {
  std::vector<Point> vec;
  for (int i = 0; i < detectorCount; ++i)
    vec.push_back(detector[i]->getPosition());
  std::sort(vec.begin(), vec.end());
  if (detectorState.insert(std::make_pair(detectorCount, vec)).second == false)
    return false;

  for (int dire = 0; dire < 4; ++dire) {
    std::sort(vec.begin(), vec.end());
    detectorState.insert(std::make_pair(detectorCount, vec));

    for (int i = 0; i < detectorCount; ++i) horizontal(vec[i]);
    std::sort(vec.begin(), vec.end());
    detectorState.insert(std::make_pair(detectorCount, vec));

    for (int i = 0; i < detectorCount; ++i) vertical(vec[i]);
    std::sort(vec.begin(), vec.end());
    detectorState.insert(std::make_pair(detectorCount, vec));
    
    for (int i = 0; i < detectorCount; ++i) horizontal(vec[i]);
    std::sort(vec.begin(), vec.end());
    detectorState.insert(std::make_pair(detectorCount, vec));

    for (int i = 0; i < detectorCount; ++i) vertical(vec[i]);
    for (int i = 0; i < detectorCount; ++i) rotate(vec[i]);
  }
  return true;
}

void PlaceState::clearDispenser() {
  dispenserState.clear();
}

void PlaceState::clearSink() {
  sinkState.clear();
}

void PlaceState::clearDetector() {
  detectorState.clear();
}

void PlaceState::set(int r_, int c_) {
  r = r_, c = c_;
}

PlaceState *placeState;