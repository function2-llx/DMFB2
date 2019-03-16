#include <cassert>
#include <vector>
#include "grid/Grid.h"
#include "grid/Cell.h"
#include "math_models/Point.h"
#include "math_models/Direction.h"
#include "placement/placement.h"

using namespace std;

bool Grid::placeDetector(Detector* detector, Point position)
{
    Cell* cell = this->get_cell(position);
    if (cell->existDetector()) return false;
    cell->setDetector(detector);
    return true;
}

bool Grid::placeSink(Sink* sink, Point position)
{
    Cell* cell = this->get_cell(position);
    // assert(cell != nullptr);
    if (cell->existSink())
        return false;
    cell->setSink(sink);
    return true;
}

void Grid::removeSink(Point position)
{
    Cell* cell = this->get_cell(position);
    cell->removeSink();
}

void Grid::removeDetector(Point position)
{
    Cell *cell = this->get_cell(position);
    cell->removeDetector();
}

Point Grid::boundaryPosition(int identifier, int type)
{
    assert(0 <= identifier && identifier < this->boundarySize[type]);
    switch(type) {
        case 0:
        return Point(identifier, 0);
        case 1:
        return Point(0, identifier);
        case 2:
        return Point(identifier, this->columns - 1);
        case 3:
        return Point(this->rows - 1, identifier);
        default:
        throw runtime_error("access error");
    }
}

#ifdef DEBUG

#include <iostream>

#endif

Grid::Grid(int rows, int columns) : rows(rows), columns(columns)
{

    this->boundarySize[(int)OuterPos::LEFT] = this->boundarySize[(int)OuterPos::RIGHT] = this->rows;
    this->boundarySize[(int)OuterPos::DOWN] = this->boundarySize[(int)(OuterPos::UP)] = this->columns;
}

void Grid::build()
{
    // this->cell = new Cell*[this->area()];
    // int cur = 0;
    // for (int i = 0; i < this->rows; i++) {
    //     for (int j = 0; j < this->columns; j++) {
    //         Point position(i, j);
    //         assert(0 <= cur && cur < this->area());
    //         this->cell[cur] = new Cell();
    //         this->cell[cur]->setPosition(position);
    //         cur++;
    //     }
    // }
    this->inner_cells.resize(rows, vector<Cell*>(columns));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            inner_cells[i][j] = new Cell();
            inner_cells[i][j]->setPosition(Point(i, j));
        }
    }
}

int Grid::area() { return this->rows * this->columns; }

bool Grid::inside(const Point& pos) const
{
    return 0 <= pos.r && pos.r < this->rows && 0 <= pos.c && pos.c < this->columns;
}

int Grid::getPointIdentifier(const Point& pos)
{
    assert(this->inside(pos));
    return pos.r * this->columns + pos.c;
}

Cell* Grid::get_cell(const Point& pos)
{
    if (inside(pos))
        return inner_cells[pos.r][pos.c];

    return nullptr;
}

const Cell* Grid::get_cell(const Point& pos) const
{
    if (inside(pos))
        return inner_cells[pos.r][pos.c];

    return nullptr;
}

Grid::~Grid()
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            delete inner_cells[i][j];
}

int Grid::getRows() { return this->rows; }

#include <cassert>

int Grid::getColumns()
{
    assert(this != nullptr);
    return this->columns;
}

void Grid::set_placement(const Placement& placement)
{
    for (auto detector_pos: placement.detector_positions)
        this->placeDetector(detector_pos.first, detector_pos.second);

    for (auto sink_pos: placement.sink_positions)
        this->placeSink(sink_pos.first, get_target_pos(sink_pos.second));
}

using OuterId = Grid::OuterId;

OuterId Grid::get_outer_id(const Point& pos) const
{
    if (pos.r == -1)
        return std::make_pair(OuterPos::UP, pos.c);

    if (pos.r == rows)
        return std::make_pair(OuterPos::DOWN, pos.c);

    if (pos.c == -1)
        return std::make_pair(OuterPos::LEFT, pos.r);

    if (pos.c == columns)
        return std::make_pair(OuterPos::RIGHT, pos.r);

    throw "wrong position";
}

Point Grid::get_pos(const OuterId& outer_id) const
{
    OuterPos pos = outer_id.first;
    int id = outer_id.second;
    switch (pos) {
        case OuterPos::LEFT:
            return Point(id, -1);
        break;

        case OuterPos::UP:
            return Point(-1, id);
        break;

        case OuterPos::RIGHT:
            return Point(id, columns);
        break;

        case OuterPos::DOWN:
            return Point(rows, id);
        break;
    }
}
Point Grid::get_target_pos(const Point& pos) const
{
    if (pos.r == -1)
        return Point(0, pos.c);

    if (pos.c == -1)
        return Point(pos.r, 0);
    
    if (pos.r == rows)
        return Point(rows - 1, pos.c);

    if (pos.c == columns)
        return Point(pos.r, columns - 1);
}

bool Grid::pos_available(const Point& pos) const
{
    auto cell = get_cell(pos);
    return cell != nullptr && cell->is_available();
}

Grid *grid;