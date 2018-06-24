#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point {
    //coordinate in the grid
    int r, c;
    Point();
    Point(int r, int c);
    void getData(int& r,int &c);
    
    friend std::ostream& operator << (std::ostream&, const Point&);
    friend bool operator == (const Point& a, const Point& b);
    friend bool operator < (const Point& a, const Point& b);
    friend Point operator + (const Point& a, const Point& b);
    friend Point operator - (const Point& a, const Point& b);
};

int manDis(Point a, Point b);
int absSum(Point a);
bool adjacent(Point a, Point b);

#endif  //POINT_H