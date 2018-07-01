#ifndef GRID_H
#define GRID_H

#include <vector>
#include "Cell.h"
#include "Point.h"

class Grid {
private:
    int rows, columns;
    Cell** cell;

    Grid(const Grid& grid);
    Grid& operator = (const Grid& grid);
    ~Grid();
public:
    int boundarySize[4];

    Grid(int rows, int columns);
    void build();

    int area();
    int getRows();
    int getColumns();
    int getPointIdentifier(Point position);
    bool inside(Point position);
    Cell* getCell(Point position);
    bool placeSink(Sink* sink, Point position);
    void removeSink(Point position);
    bool placeDetector(Detector* detector, Point position);
    void removeDetector(Point position);
    Point boundaryPosition(int identifier, int type);
};

extern Grid* grid;

#endif  //GRID_H