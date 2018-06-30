#include <cassert>
#include <vector>
#include <iostream>
#include "Grid.h"
#include "DMFB.h"
#include "Global.h"
#include "Droplet.h"
#include "Dispenser.h"
#include "Direction.h"
#include "Detector.h"
#include "DropletData.h"

using namespace std;

Droplet::Droplet(const Droplet& droplet)
{
    this->identifier = droplet.identifier;
    this->type = droplet.type;
    this->position = droplet.position;
    this->dispensed = droplet.dispensed;
    this->mixing = droplet.mixing;
    this->remainingMixingTime = droplet.remainingMixingTime;
    if (this->remainingMixingTime == 0) {
        this->mixing = false;
    }
    this->detecting = droplet.detecting;
    this->remainingDetectingTime = droplet.remainingDetectingTime;
    if (this->remainingDetectingTime == 0) {
        this->detecting = false;
    }
}

Droplet::Droplet(const DropletData& dropletData)
{
    this->setData(dropletData);
    this->dispensed = false;
    this->mixing = false;
    this->detecting = false;
    this->position = dispenser[this->type]->getPosition();
}

Droplet::Droplet(const Droplet* precursor, const Direction& direction)
{
    this->identifier = precursor->identifier;
    this->type = precursor->type;
    this->position = precursor->position + direction;
    assert(grid->inside(this->position));
    this->detecting = precursor->detecting;
    this->mixing = precursor->mixing;
    this->dispensed = true;
    if (precursor->mixing) {
        this->remainingMixingTime = precursor->remainingMixingTime - 1;
    } else {
        this->remainingMixingTime = 0;
    }
    if (precursor->detecting) {
        assert(direction == zeroDirection);
        this->remainingDetectingTime = precursor->remainingDetectingTime - 1;
    } else {
        this->remainingDetectingTime = precursor->remainingDetectingTime;
    }
}

Droplet::Droplet(const Droplet& droplet1, const Droplet& droplet2)
{
    using namespace Global;
    assert(!droplet1.underMixing());
    assert(!droplet1.underDetection());
    assert(!droplet2.underMixing());
    assert(!droplet2.underDetection());
    assert(droplet1.position == droplet2.position);
    assert(grid->inside(droplet1.position));
    assert(mixingResult[droplet1.identifier][droplet2.identifier] != -1);
    this->setData(dropletData[mixingResult[droplet1.identifier][droplet2.identifier]]);
    this->dispensed = true;
    this->remainingMixingTime--;
    this->mixing = true;
    this->detecting = false;
    this->position = droplet1.position;
}

void Droplet::setData(const DropletData& dropletData)
{
    this->identifier = dropletData.identifier;
    this->type = dropletData.type;
    this->remainingMixingTime = dropletData.mixingTime;
    this->remainingDetectingTime = dropletData.detectingTime;
}

ULL Droplet::hash() const
{
    static ULL hashBase = 894137589146ull;
    static ULL shift = 7891746412ull;
    ULL ret = this->identifier;
    ret = grid->getPointIdentifier(this->position) + shift + hashBase * ret;
    ret = this->dispensed + shift + hashBase * ret;
    ret = this->mixing + shift + hashBase * ret;
    ret = this->remainingMixingTime + shift + hashBase * ret;
    ret = this->detecting + shift + hashBase * ret;
    ret = this->remainingDetectingTime + shift + hashBase * ret;
    return ret;
}

bool Droplet::inGrid() const
{
    return this->dispensed;
}

bool Droplet::underMixing() const
{
    return this->mixing;
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

bool Droplet::underDetection() const
{
    return this->detecting;
}

bool Droplet::mixed() const
{
    return !this->mixing;
}

bool Droplet::detected() const
{
    return this->detecting == false && this->remainingDetectingTime == 0;
}

int Droplet::getIdentifier() const
{
    return this->identifier;
}

int Droplet::getType() const
{
    return this->type;
}

Point Droplet::getPosition() const
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
    if (droplet.mixing) {
        os << "mixing, " << droplet.remainingMixingTime;
        if (droplet.remainingMixingTime == 1) {
            os << " step left" << endl;
        } else {
            os << " steps left" << endl;
        }
    }
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

int Droplet::estimatedTime() const
{
    int ret = leastTime[this->identifier];
    if (!this->detected()) {
        ret += this->remainingDetectingTime;
        if (!this->underDetection()) {
            ret += max(manDis(this->position, detectorPosition[this->type]), this->remainingMixingTime);
        }
    }
    if (!this->dispensed) ret++;
    return ret;
}

bool Droplet::isEndDroplet() const
{
    return !Global::toBeMixed[this->identifier];
}