#include "Point.h"

using namespace std;

Point::Point() {}

Point::Point(int r, int c) : r(r), c(c) {}

void Point::getData(int& r, int &c) {
    r = this->r;
    c = this->c;
}

ostream& operator << (ostream& os, const Point& point)
{
    os << "(" << point.r << ", " << point.c << ")";
    return os;
}

Point operator + (const Point& a, const Point& b)
{
    return Point(a.r + b.r, a.c + b.c);
}

bool operator == (const Point& a, const Point& b)
{
    return a.r == b.r && a.c == b.c;
}

Point operator - (const Point& a, const Point& b)
{
    return Point(a.r - b.r, a.c - b.c);
}

// int abs(int x)
// {
//     return x > 0 ? x : -x;
// }

int absSum(Point a)
{
    return abs(a.r) + abs(a.c);
}

int manDis(Point a, Point b)
{
    return absSum(a-b);
}

bool adjacent(Point a, Point b)
{
    return abs(a.r - b.r) <= 1 && abs(a.c - b.c) <= 1;
}