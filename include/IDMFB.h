#pragma once

#include "math_models/Point.h"
#include <vector>
#include <string>

namespace IDMFB {
    struct MoveSequence {
        int droplet_id, t;
        std::vector<Point> route;
    };

    std::vector<MoveSequence> get_move_sequences(const std::string& filename, int n, int m);

}

extern std::ostream& operator << (std::ostream& os, const IDMFB::MoveSequence& seq);