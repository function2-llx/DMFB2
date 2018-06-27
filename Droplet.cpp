#include <cassert>
#include <vector>
#include <iostream>
#include "Grid.h"
#include "DMFB.h"
#include "Droplet.h"
#include "Dispenser.h"
#include "Direction.h"
#include "Detection.h"
#include "Detector.h"

using namespace std;

Droplet::Droplet(Droplet* precursor, Direction direction)
{
    this->identifier = precursor->identifier;
    this->type = precursor->getType();
    this->position = precursor->getPosition() + direction;
    this->detecting = precursor->detecting;
    assert(grid->inside(this->position));
    if (precursor->underDetection()) {
        assert(direction == zeroDirection);
        this->remainingDetectingTime = precursor->remainingDetectingTime - 1;
        if (this->remainingDetectingTime == 0) this->detecting = false;
    } else {
        this->remainingDetectingTime = precursor->remainingDetectingTime;
    }
}

ULL Droplet::hash()
{
    static ULL hashBase = 894137589146ull;
    static ULL shift = 7891746412ull;
    ULL ret = this->identifier;
    ret = grid->getPointIdentifier(this->position) + shift + hashBase * ret;
    ret = this->detecting + shift + hashBase * ret;
    ret = this->remainingDetectingTime + shift + hashBase * ret;
    return ret;
}

void Droplet::startDetection()
{
    assert(this->detecting == false);
    this->remainingDetectingTime--;
    if (this->remainingDetectingTime == 0) {
        this->detecting = false;
    } else {
        this->detecting = true;
    }
}

bool Droplet::underDetection()
{
    return this->detecting;
}

bool Droplet::detected() const
{
return this->detecting == false && this->remainingDetectingTime == 0;
}

int Droplet::getIdentifier()
{
    return this->identifier;
}

int Droplet::getType()
{
    return this->type;
}

Point Droplet::getPosition()
{
    return this->position;
}

ostream& operator << (ostream& os, const Droplet& droplet)
{
    os << "droplet" << endl;
	os << "identifier: " << droplet.identifier << endl;
	os << "type: " << droplet.type << endl;
	os << "position: " << droplet.position << endl;
	os << "detecting state: ";
	if (droplet.detected()) {
		os << "has been detected" << endl;
	} else {
		if (droplet.detecting) {
			os << "detecting, ";
			if (droplet.remainingDetectingTime > 1) {
				os << droplet.remainingDetectingTime << " steps left" << endl;
			} else {
				os << "1 step lefts" << endl;
			}
		} else {
			os << "has not started yet" << endl;
		}
	}
    return os;
}

int Droplet::estimatedTime()
{
    int ret = leastTime[this->identifier];
    if (!this->detected()) {
        ret += this->remainingDetectingTime;
        if (!this->underDetection()) {
            ret += manDis(this->position, detectorPosition[this->type]);
        }
    }
    return ret;
}