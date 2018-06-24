#pragma once

#include "Droplet.h"
#include "Mixer.h"
#include "Point.h"

class GridRecorder {
private:
	int rows, columns;
	/*
	0: nothing
	1: occupied by a droplet
	2: influenced by a droplet
	3: mixer
	*/
	int** type;
	int** identifier;
	void occupy(Point upperLeftCorner, Point lowerRightCorner, int occupyType, int occupyIdentifier);
public:
	GridRecorder(int rows, int columns);
	GridRecorder(const GridRecorder&);
	~GridRecorder();
	void addDroplet(Droplet*);
	void addMixer(Mixer*);
	int occupyType(Point position);
	int occupyIdentifier(Point position);
};