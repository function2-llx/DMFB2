#include <cassert>
#include <vector>
#include <iostream>
#include "grid/Grid.h"
#include "core/DMFB.h"
#include "Global.h"
#include "droplet/Droplet.h"
#include "entities/Dispenser.h"
#include "math_models/Direction.h"
#include "entities/Detector.h"
#include "droplet/DropletData.h"

using namespace std;

Droplet::Droplet(const Droplet& droplet)
{
    this->identifier = droplet.identifier;
    this->type = droplet.type;
    this->position = droplet.position;
    this->dispensed = droplet.dispensed;
    this->mixing = droplet.mixing;
    this->remainingMixingTime = droplet.remainingMixingTime;
    this->detecting = droplet.detecting;
    this->remainingDetectingTime = droplet.remainingDetectingTime;
    
    // if (this->remainingMixingTime == 0) {
    //     this->mixing = false;
    // }


    // if (this->remainingDetectingTime == 0) {
    //     this->detecting = false;
    // }
}

Droplet::Droplet(const DropletData& dropletData)
{
    this->setData(dropletData);
    this->dispensed = false;
    this->mixing = false;
    this->detecting = false;
    this->position = DMFBsolver->get_dispenser(type)->getPosition();
}

Droplet::Droplet(const Droplet* precursor, const Direction& direction) : Droplet(*precursor)
{
    // *this = *precursor;
    this->dispensed = true;
    position = position + direction;
    this->time_past(1);
    // this->identifier = precursor->identifier;
    // this->type = precursor->type;
    // this->position = precursor->position + direction;
    // assert(grid->inside(this->position));
    // this->detecting = precursor->detecting;
    // this->mixing = precursor->mixing;

    // if (precursor->mixing) {
    //     this->remainingMixingTime = precursor->remainingMixingTime - 1;
    // } else {
    //     this->remainingMixingTime = 0;
    // }
    // if (precursor->detecting) {
    //     assert(direction == zeroDirection);
    //     this->remainingDetectingTime = precursor->remainingDetectingTime - 1;
    // } else {
    //     this->remainingDetectingTime = precursor->remainingDetectingTime;
    // }
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

    int result_id = DMFBsolver->get_mixing_result_id(&droplet1, &droplet2);
    assert(result_id != -1);

    this->setData(DMFBsolver->get_droplet_data(result_id));
    this->dispensed = true;
    // this->remainingMixingTime--;
    this->mixing = true;
    this->detecting = false;
    this->position = droplet1.position;

    // time_past(1);
}

//  deal with remaining detection and mixing time
void Droplet::time_past(int t)
{
    if (this->detecting) {
        if (remainingDetectingTime <= t) {
            detecting = 0;
            remainingDetectingTime = 0;
        } else
            remainingDetectingTime -= t;
    }

    if (this->mixing) {
        if (remainingMixingTime <= t) {
            mixing = 0;
            remainingMixingTime = 0;
        } else 
            remainingMixingTime -= t;
    }
}

// Droplet* Droplet::get_moved_droplet(const Direction& direction) const
// {
//     Droplet* ret = new Droplet(*this);
//     ret->position = ret->position + direction;
//     ret->time_past(1);
//     return ret;
// }

bool operator == (const Droplet& a, const Droplet& b)
{
    if (a.identifier != b.identifier)
        return 0;

    if (a.dispensed != b.dispensed)
        return 0;

    if (a.position != b.position)
        return 0;

    if (a.mixing != b.mixing || a.remainingMixingTime != b.remainingMixingTime)
        return 0;

    if (a.detecting != b.detecting || a.remainingDetectingTime != b.remainingDetectingTime)
        return 0;
    
    return 1;
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

bool Droplet::is_dispensed() const { return this->dispensed; }

bool Droplet::underMixing() const { return this->mixing; }

void Droplet::startDetection()
{
    assert(this->detecting == false);
    this->detecting = 1;
    // this->time_past(1);
    // this->remainingDetectingTime--;
    // if (this->remainingDetectingTime == 0) {
    //     this->detecting = false;
    // } else {
    //     this->detecting = true;
    // }
}

bool Droplet::underDetection() const { return this->detecting; }

bool Droplet::mixed() const { return !this->mixing; }

bool Droplet::detected() const
{
    // return this->detecting == false && this->remainingDetectingTime == 0;
    return remainingDetectingTime == 0;
}

int Droplet::getType() const { return this->type; }

Point Droplet::getPosition() const { return this->position; }

ostream& operator << (ostream& os, const Droplet& droplet)
{
    os << "droplet" << endl;
	os << "identifier: " << droplet.identifier << endl;
	os << "type: " << DMFBsolver->get_real_type(droplet.type) << endl;
	os << "position: " << droplet.position << endl;
	// os << "mixing state: ";
    if (droplet.mixing) {
        os << "mixing, " << droplet.remainingMixingTime;
        if (droplet.remainingMixingTime == 1) {
            os << " step left" << endl;
        } else {
            os << " steps left" << endl;
        }
    }
    // else
    //     cerr << "has not started yet" << endl;

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

int Droplet::estimatedTime() const
{
    int ret = DMFBsolver->get_least_time(this);
    if (!this->detected()) {
        ret += this->remainingDetectingTime;
        if (!this->underDetection()) {
            // ret += max(manDis(this->position, detectorPosition[this->type]), this->remainingMixingTime);
            ret += max(manDis(this->position, DMFBsolver->get_detector(type)->get_pos()), this->remainingMixingTime);
        }
    }
    if (!this->dispensed) ret++;
    return ret;
}

bool Droplet::isEndDroplet() const { return !DMFBsolver->is_to_mix(this); }

// Droplet* Droplet::merge(const Droplet* a, const Droplet* b) 
// {
//     auto ret = new Droplet(*a, *b); 
//     ret->time_past(1);
//     return ret;
// }