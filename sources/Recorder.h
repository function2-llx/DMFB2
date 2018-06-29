#pragma once

#include "Point.h"

class Recorder {
private:
	int record[3][3];
public:
	void backup(int** data, Point position);
	void restore(int** data, Point position) const;
};