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

int manDis(Point a, Point b);
int absSum(Point a);
bool adjacent(Point a, Point b);

std::ostream& operator << (std::ostream&, const Point&);
bool operator == (const Point& a, const Point& b);
bool operator != (const Point& a, const Point& b);
bool operator < (const Point& a, const Point& b);
Point operator + (const Point& a, const Point& b);
Point operator - (const Point& a, const Point& b);

#endif  //POINT_H