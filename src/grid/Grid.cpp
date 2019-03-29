#include <cassert>
#include <vector>
#include <queue>
#include <unordered_set>
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

Grid::Grid(int rows, int columns) : rows(rows), columns(columns), 
    dis(0, point_hasher)
{
    this->boundarySize[(int)OuterPos::LEFT] = this->boundarySize[(int)OuterPos::RIGHT] = this->rows;
    this->boundarySize[(int)OuterPos::DOWN] = this->boundarySize[(int)(OuterPos::UP)] = this->columns;
}

void Grid::build()
{
    this->inner_cells.resize(rows, vector<Cell*>(columns));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            inner_cells[i][j] = new Cell();
            inner_cells[i][j]->setPosition(Point(i, j));
        }
    }
}

void Grid::disable(const vector<Point>& disable_pos)
{
    for (auto pos: disable_pos) {
        // if (pos.r == 20) std::cerr << "disable: " << pos << std::endl;
        auto cell = get_cell(pos);
        if (cell != nullptr)
            cell->set_available(0);
    }
    this->init_dis();
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
    this->placeDetector(placement.detector_pos.first, placement.detector_pos.second);
    // for (auto detector_pos: placement.detector_positions)
    //     this->placeDetector(detector_pos.first, detector_pos.second);

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

    throw "never mind scandal and liber";
}

bool Grid::pos_available(const Point& pos) const
{
    auto cell = get_cell(pos);
    return cell != nullptr && cell->is_available();
}

int Grid::get_dis(const Point& a, const Point& b) const
{
    // return man_dis(a, b);
    return dis.at(a).at(b);
    // assert(grid->pos_available(a));
    // auto test = dis.at(a);
    // cerr << b << endl;
    // assert(grid->pos_available(b));

    // return test.at(b);
}

void Grid::bfs(const Point& s)
{
    using namespace std;
    // if (s.r == 30)
    //     cerr << s << endl;
    std::unordered_map<Point, int, PointHash> &dis = 
        this->dis[s] = std::unordered_map<Point, int, PointHash>(0, point_hasher);
    dis[s] = 0;
    unordered_set<Point, PointHash> vis(0, point_hasher);
    queue<Point> que;
    que.push(s);
    vis.insert(s);
    dis[s] = 0;
    while (!que.empty()) {
        auto u = que.front();
        que.pop();
        int d = dis[u];
        for (auto dir: ::direction) {
            auto v = u + dir;
            if (this->pos_available(v) && !vis.count(v)) {
                dis[v] = d + 1;
                vis.insert(v);
                que.push(v);
            }
        }
    }
}

void Grid::init_dis()
{
    for (int i = 0; i < this->rows; i++)
        for (int j = 0; j < this->columns; j++) {
            Point cur(i, j);
            if (this->pos_available(cur))
                bfs(cur);
        }
}

Grid *grid;