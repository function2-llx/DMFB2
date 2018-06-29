#include <cassert>
#include "GridRecorder.h"
#include "Grid.h"

GridRecorder::GridRecorder(int rows, int columns) : rows(rows), columns(columns)
{
	type = new int*[rows];
	this->identifier = new int*[rows];
	for (int i = 0; i < rows; i++) {
		type[i] = new int[columns];
		this->identifier[i] = new int[columns];
		for (int j = 0; j < columns; j++) {
			type[i][j] = 0;
		}
	}
}

GridRecorder::GridRecorder(const GridRecorder& other) : rows(other.rows), columns(other.columns)
{
	type = new int*[rows];
	this->identifier = new int*[rows];
	for (int i = 0; i < rows; i++) {
		this->type[i] = new int[this->columns];
		this->identifier[i] = new int[this->columns];
		for (int j = 0; j < columns; j++) {
			this->type[i][j] = other.type[i][j];
			this->identifier[i][j] = other.identifier[i][j];
		}
	}
}

GridRecorder::~GridRecorder()
{
	for (int i = 0; i < this->rows; i++) {
		delete[] type[i];
		delete[] identifier[i];
	}
	delete[] identifier;
	delete[] type;
}

void GridRecorder::occupy(Point upperLeftCorner, Point lowerRightCorner, int type, int identifier)
{
	for (int i = upperLeftCorner.r; i <= lowerRightCorner.r; i++) {
		for (int j = upperLeftCorner.c; j <= lowerRightCorner.c; j++) {
			Point position(i, j);
			if (grid->inside(position)) {
				this->type[i][j] = type;
				this->identifier[i][j] = identifier;
			}
		}
	}
}

void GridRecorder::addDroplet(Droplet* droplet)
{
	Point position = droplet->getPosition();
	assert(grid->inside(position));
	this->occupy(position - Direction(1, 1), position + Direction(1, 1), 2, droplet->getIdentifier());
	this->type[position.r][position.c] = 1;
}

void GridRecorder::addMixer(Mixer* mixer)
{
	this->occupy(
		mixer->getUpperleftCorner() - Direction(1, 1), 
		mixer->getLowerRightCorner() + Direction(1, 1), 
		3, 
		mixer->getIdentifier());
}

int GridRecorder::occupyType(Point position)
{
	return this->type[position.r][position.c];
}

int GridRecorder::occupyIdentifier(Point position)
{
	return this->identifier[position.r][position.c];
}
