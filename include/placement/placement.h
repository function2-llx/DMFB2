#pragma once

#include <vector>

class Detector;
class Sink;
class Dispenser;
struct Point;

struct Placement {
    using DetectorPos = std::pair<Detector*, Point>;
    using SinkPos = std::pair<Sink*, Point>;
    using DispenserPos = std::pair<Dispenser*, Point>;

    std::vector<DetectorPos> detector_positions;
    std::vector<SinkPos> sink_positions;
    std::vector<DispenserPos> dispenser_positions;

    Placement(const Placement&) = default;
    Placement() = default;

    friend std::ostream& operator << (std::ostream&, const Placement&);
};