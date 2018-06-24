#ifndef LINE_H
#define LINE_H

#include "Point.h"

struct Line {
	int c, r1, r2, type;
	Line(int c = 0, int r1 = 0, int r2 = 0, int type = 0);
	bool operator < (const Line& a) const;
};

void getLine(Point a, Point b, Line* line);

#endif	//LINE_H