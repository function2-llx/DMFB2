#ifndef GRID_H
#define GRID_H

#include <vector>
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
};

extern Grid* grid;

#endif  //GRID_H