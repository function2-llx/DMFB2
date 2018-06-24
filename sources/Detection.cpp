#include <iostream>
#include "Detection.h"

using namespace std;

istream& operator >> (istream& is, Detection& detection)
{
	is >> detection.duration;
	return is;
}

Detection** detection;