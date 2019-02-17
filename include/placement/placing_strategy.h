#pragma once

#include <vector>
#include "placement/placement.h"

class Dispenser;
class Sink;
class Detector;

class PlacingStrategy {
  public:
    virtual ~PlacingStrategy() {}

    virtual Placement get_placement(
        const std::vector<Dispenser*>&, 
        const std::vector<Sink*>&,
        const std::vector<Detector*>&,
        int rows, int columns) const = 0;

};