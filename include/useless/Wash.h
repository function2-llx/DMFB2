#pragma once

#include "math_models/Point.h"

struct Wash {
	Point position;	//清洗位置
	int t1, t2;	//清洗最晚时间
	Wash() {}
	Wash(Point &position_, int t1_, int t2_):
		position(position_), t1(t1_), t2(t2_) {}
};