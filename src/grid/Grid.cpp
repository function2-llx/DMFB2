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

bool Grid::inside(Point position)
{
    static int r, c;   
    position.getData(r, c);
    return 0 <= r && r < this->rows && 0 <= c && c < this->columns;
}

int Grid::getPointIdentifier(Point position)
{
    assert(this->inside(position));
    return position.r * this->columns + position.c;
}

Cell* Grid::get_cell(Point position)
{
    if (inside(position))
        return inner_cells[position.r][position.c];

    return nullptr;
}

Grid::~Grid()
{
    // for (int i = 0; i < this->area(); i++) {
    //     delete cell[i];
    // }
    // delete []cell;
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
        this->placeSink(sink_pos.first, sink_pos.second);

}

Grid *grid;