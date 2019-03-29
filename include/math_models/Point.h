#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point {
    //coordinate in the grid
    int r, c;
    Point();
    Point(int r, int c);
    void getData(int& r,int &c);
};

// class Grid;
// extern Grid *grid;

// namespace std {
//     template<>
//     struct hash<Point> {
//         size_t operator () (const Point& a) const {
//             return grid->getPointIdentifier(a);
//         }
//     };
// }


//Manhattan distance
int man_dis(const Point& a, const Point& b);
int one_nore(const Point& a);
bool adjacent(const Point& a, const Point& b);

std::ostream& operator << (std::ostream&, const Point&);
bool operator == (const Point& a, const Point& b);
bool operator != (const Point& a, const Point& b);
bool operator < (const Point& a, const Point& b);
Point operator + (const Point& a, const Point& b);
Point operator - (const Point& a, const Point& b);

#endif  //POINT_H