#include "placement/placing_strategy.h"

class RandomPlacingStrategy : public PlacingStrategy {
  private:
    int seed;
  public:
    RandomPlacingStrategy(int seed = 2333333) : seed(seed) {}

    Placement get_placement(
        const std::vector<Dispenser*>&, 
        const std::vector<Sink*>&,
        // const std::vector<Detector*>&,
        Detector*,
        int rows, int columns) const;

};