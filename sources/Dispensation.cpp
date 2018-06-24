#include <iostream>
#include "Dispensation.h"
#include "Point.h"
#include "Dispenser.h"
#include "Detector.h"
#include "DMFB.h"

using namespace std;

Dispensation::Dispensation(int identifier, int type)
{
	this->identifier = identifier;
	this->type = type;
}

ostream& operator << (ostream& os, const Dispensation& dispensation)
{
	os << "dispensation" << endl;
	os << "corresponding droplet identifier: " << dispensation.identifier << endl;
	os << "type: " << dispensation.type << endl;
	return os;
}

int Dispensation::estimatedTime()
{
	int ret = 1;
	if (detection[this->identifier]->duration != 0) {
		ret += manDis(dispenser[this->type]->getPosition(), detectorPosition[this->type])
			+ detection[this->identifier]->duration;
	}
	return ret + leastTime[this->identifier];
}

std::vector<Dispensation*> allDispensations;