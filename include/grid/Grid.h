#ifndef GRID_H
#define GRID_H

#include <vector>
#include <unordered_map>
#include <functional>
#include "grid/Cell.h"
#include "math_models/Point.h"

struct Placement;

class Grid {
  private:
    int rows, columns;
    // Cell** cell;
    std::vector<std::vector<Cell*> > inner_cells;

    explicit Grid(const Grid& grid);
    Grid& operator = (const Grid& grid);
    ~Grid();

    // using PointHash = std::function<size_t(const Point&)>;
    std::function<size_t(const Point&)> point_hasher = [this] (const Point& pos) -> size_t {
        return this->getPointIdentifier(pos);
    };

    using PointHash = decltype(point_hasher);

    std::unordered_map<Point, std::unordered_map<Point, int, PointHash>, PointHash> dis;

    void init_dis();
    void bfs(const Point& pos);
    
  public:
    enum struct OuterPos: int {
        LEFT  = 0,
        UP    = 1,
        RIGHT = 2,
        DOWN  = 3
    };

    using OuterId = std::pair<OuterPos, int>;
    OuterId get_outer_id(const Point&) const;
    Point get_pos(const OuterId& outer_Id) const;
    Point get_target_pos(const Point& pos) const;   //  return the position a sink/dispenser connect to

    int boundarySize[4];

    Grid(int rows, int columns);
    void build();
    
    int area();
    int getRows();
    int getColumns();
    int getPointIdentifier(const Point&);
    bool inside(const Point&) const;
    Cell* get_cell(const Point&);
    const Cell* get_cell(const Point&) const;

    bool placeSink(Sink* sink, Point position);
    void removeSink(Point position);
    bool placeDetector(Detector* detector, Point position);
    void removeDetector(Point position);
    Point boundaryPosition(int identifier, int type);

    void set_placement(const Placement&);

    void disable(const std::vector<Point>& disable_pos);
    bool pos_available(const Point&) const;

    int get_dis(const Point&, const Point&) const;

    bool is_mixing_area(const Point& a) const { return a.r % 7 < 3 && a.c % 7 < 3; }
    bool is_moving_area(const Point& a) const { return this->pos_available(a) && !this->is_mixing_area(a); }
};

extern Grid* grid;

// namespace std {
//     template<>
//     struct hash<Point> {
//         size_t operator () (const Point& a) const {
//             return grid->getPointIdentifier(a);
//         }
//     };
// }

#endif  //GRID_H