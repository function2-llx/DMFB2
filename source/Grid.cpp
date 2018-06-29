#include <cassert>
#include <vector>
#include "Grid.h"
#include "Cell.h"
#include "Point.h"
#include "Direction.h"

using namespace std;

bool Grid::placeDetector(Detector* detector, Point position)
{
    Cell* cell = this->getCell(position);
    if (cell->existDetector()) return false;
    cell->setDetector(detector);
    return true;
}


bool Grid::placeSink(Sink* sink, Point position)
{
    Cell* cell = this->getCell(position);
    if (cell->existSink()) return false;
    cell->setSink(sink);
    return true;
}

void Grid::removeSink(Point position)
{
    Cell* cell = this->getCell(position);
    cell->removeSink();
}

void Grid::removeDetector(Point position)
{
    Cell *cell = this->getCell(position);
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
        return Point(this->rows -1, identifier);
        default:
        assert(false);
    }
}

#ifdef DEBUG

#include <iostream>

#endif

Grid::Grid(int rows, int columns) : rows(rows), columns(columns)
{
    this->boundarySize[0] = this->boundarySize[2] = this->rows;
    this->boundarySize[1] = this->boundarySize[3] = this->columns;
}

void Grid::build()
{
    this->cell = new Cell*[this->area()];
    int cur = 0;
    for (int i = 0; i < this->rows; i++) {
        for (int j = 0; j < this->columns; j++) {
            Point position(i, j);
            assert(0 <= cur && cur < this->area());
            this->cell[cur] = new Cell();
            this->cell[cur]->setPosition(position);
            cur++;
        }
    }
}

int Grid::area()
{
    return this->rows * this->columns;
}

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

Cell* Grid::getCell(Point position)
{
    assert(grid->inside(position));
    return cell[this->getPointIdentifier(position)];
}

Grid::~Grid()
{
    for (int i = 0; i < this->area(); i++) {
        delete cell[i];
    }
    delete []cell;
}

int Grid::getRows()
{
    return this->rows;
}

#include <cassert>

int Grid::getColumns()
{
    assert(this != nullptr);
    return this->columns;
}

Grid* grid;