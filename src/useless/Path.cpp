#include "useless/Path.h"



Path::Path()
{
	this->path.clear();
}

void Path::insert(int x)
{
	this->path.push_back(x);
}
