#include <cassert>

#include "grid/Grid.h"

#include "placement/placement.h"
#include "placement/random_placing_strategy.h"

Placement RandomPlacingStrategy::get_placement(
    const std::vector<Dispenser*>& dispensers,
    const std::vector<Sink*>& sinks,
    const std::vector<Detector*>& detectors,
    int rows, int columns) const
{
    Placement placement;
    placement.dispenser_positions.reserve(dispensers.size());
    placement.detector_positions.reserve(detectors.size());
    placement.sink_positions.reserve(sinks.size());

    using OuterPos = Grid::OuterPos;
    auto get_size = [rows, columns] (OuterPos pos) { return (pos == OuterPos::LEFT || pos == OuterPos::RIGHT) ? rows : columns; };

    assert(dispensers.size() + sinks.size() <= (rows + columns) * 2);
    assert(detectors.size() <= rows * columns);

    bool *outer_vis[4];
    int size[4];
    for (int i = 0; i < 4; i++) {
        OuterPos pos = static_cast<OuterPos>(i);
        size[i] = get_size(pos);
        outer_vis[i] = new bool[size[i]];
        for (int j = 0; j < size[i]; j++)
            outer_vis[i][j] = 0;
    }
    
    srand(this->seed);

    auto get_random_outer_pos = [size, outer_vis] {
        int outer = rand() % 4, pos = rand() % size[outer];
        while (outer_vis[outer][pos])
            outer = rand() % 4, pos = rand() % size[outer];

        return std::make_pair(outer, pos);
    };

    // auto get_pos = [rows, columns] (OuterPos pos, int id) {
    //     switch (pos) {
    //         case OuterPos::LEFT:
    //             return Point(id, -1);
    //         break;

    //         case OuterPos::UP:
    //             return Point(-1, id);
    //         break;

    //         case OuterPos::RIGHT:
    //             return Point(id, columns);
    //         break;

    //         case OuterPos::DOWN:
    //             return Point(rows, id);
    //         break;
    //     }
    // };

    for (auto dispenser: dispensers) {
        auto outer_pos = get_random_outer_pos();
        int outer = outer_pos.first, pos = outer_pos.second;
        outer_vis[outer][pos] = 1;
        placement.dispenser_positions.push_back(std::make_pair(dispenser, grid->get_pos(std::make_pair(static_cast<OuterPos>(outer), pos))));
    }

    for (auto sink: sinks) {
        auto outer_pos = get_random_outer_pos();
        int outer = outer_pos.first, pos = outer_pos.second;
        outer_vis[outer][pos] = 1;
        placement.sink_positions.push_back(std::make_pair(sink, grid->get_pos(std::make_pair(static_cast<OuterPos>(outer), pos))));
    }
    for (int i = 0; i < 4; i++)
        delete[] outer_vis[i];

    bool inner_vis[rows][columns]{};
    for (auto detector: detectors) {
        int r = rand() % rows, c = rand() % columns;
        while (inner_vis[r][c])
            r = rand() % rows, c = rand() % columns;

        inner_vis[r][c] = 1;
        placement.detector_positions.push_back(std::make_pair(detector, Point(r, c)));
    }

    return placement;
}