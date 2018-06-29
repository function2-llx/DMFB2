#include <cassert>
#include "Line.h"
#include "Point.h"

Line::Line(int c, int r1, int r2, int type) : c(c), r1(r1), r2(r2), type(type)
{

}

bool Line::operator < (const Line& line) const
{
	if (this->c == line.c) return this->type == 1;
	return this->c < line.c;
}
void getLine(Point a, Point b, Line* line)
{
	assert(a.r <= b.r);
	assert(a.c <= b.c);
	line[0] = Line(a.c, a.r, b.r);
	line[1] = Line(b.c, a.r, b.r);
}
