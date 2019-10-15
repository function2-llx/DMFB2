#include <cassert>

#include "grid/Grid.h"

#include "placement/placement.h"
#include "placement/random_placing_strategy.h"
#include "grid/Grid.h"

Placement RandomPlacingStrategy::get_placement(
    const std::vector<Dispenser*>& dispensers,
    const std::vector<Sink*>& sinks,
    // const std::vector<Detector*>& detectors,
    Detector* detector,
    int rows, int columns) const {
    Placement placement;
    placement.dispenser_positions.reserve(dispensers.size());
    // placement.detector_positions.reserve(detectors.size());
    placement.sink_positions.reserve(sinks.size());

    using OuterPos = Grid::OuterPos;
    auto get_size = [rows, columns] (OuterPos pos) { return (pos == OuterPos::LEFT || pos == OuterPos::RIGHT) ? rows : columns; };

    assert(dispensers.size() + sinks.size() <= (rows + columns) * 2);
    // assert(detectors.size() <= rows * columns);
    using namespace std;
    bool *outer_vis[4];
    int size[4];
    for (int i = 0; i < 4; i++) {
        OuterPos pos = static_cast<OuterPos>(i);
        size[i] = get_size(pos);
        cerr << "placing" << i << endl;
        cerr << "size: " << size[i] << endl;
        outer_vis[i] = new bool[size[i]];
        for (int j = 0; j < size[i]; j++)
            outer_vis[i][j] = 0;
    }
    
    srand(this->seed);

    auto get_random_outer_pos = [size, outer_vis] {
        int outer = rand() % 4, pos = rand() % size[outer];
        auto target_pos = grid->get_target_pos(grid->get_pos(std::make_pair(static_cast<OuterPos>(outer), pos)));
        while (outer_vis[outer][pos] || !grid->pos_available(target_pos))
            outer = rand() % 4, pos = rand() % size[outer], 
            target_pos = grid->get_target_pos(grid->get_pos(std::make_pair(static_cast<OuterPos>(outer), pos)));
        // std::cerr << target_pos << std::endl;
        assert(grid->pos_available(target_pos));

        return std::make_pair(outer, pos);
    };

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

    bool inner_vis[rows][columns];
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            inner_vis[i][j] = 0;

    for (int r = rand() % rows, c = rand() % columns; ; r = rand() % rows, c = rand() % columns) {
        if (grid->pos_available(Point(r, c)) && !inner_vis[r][c]) {
            inner_vis[r][c] = 1;
            placement.detector_pos = std::make_pair(detector, Point(r, c));
            break;
        }
    }

    // [&] {
    //     int r = rand() % rows, c = rand() % columns;
    //     while (grid->pos_available(Point(r, c)) && inner_vis[r][c])
    //         r = rand() % rows, c = rand() % columns;

    //     inner_vis[r][c] = 1;
    //     placement.detector_pos = std::make_pair(detector, Point(r, c));
    // }();

    // for (auto detector: detectors) {
    //     int r = rand() % rows, c = rand() % columns;
    //     while (grid->pos_available(Point(r, c)) && inner_vis[r][c])
    //         r = rand() % rows, c = rand() % columns;

    //     inner_vis[r][c] = 1;
    //     placement.detector_positions.push_back(std::make_pair(detector, Point(r, c)));
    // }

    return placement;
}