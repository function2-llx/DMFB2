#include "Recorder.h"
#include "Grid.h"

void Recorder::backup(int** data, Point position)
{
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (grid->inside(Point(position.r + i, position.c + j))) {
				this->record[i + 1][j + 1] = data[i][j];
			}
		}
	}
}

void Recorder::restore(int** data, Point position) const
{
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (grid->inside(Point(position.r + i, position.c + j))) {
				data[i][j] = this->record[i + 1][j + 1];
			}
		}
	}
}