#include <cassert>
#include <cstring>
#include <algorithm>
#include "Point.h"
#include "Line.h"
#include "Direction.h"
#include "Mixer.h"
#include "Grid.h"
#include "MixingOperation.h"
#include "DMFB.h"

using namespace std;

ostream& operator << (ostream& os, const Mixer& mixer)
{
    os << "mixer" << endl;
    os << "identifier: " << mixer.identifier << endl;
    os << "upper left corner: " << mixer.upperLeftCorner << endl;
    os << "lower right corner: " << mixer.lowerRightCorner << endl;
    os << "output position: " << mixer.outputPosition << endl;
    os << "remaining time: " << mixer.remainingTime << endl;
    return os;
}

Mixer::Mixer(MixingOperation* mixingOperation, Point inputPosition, Point upperLeftCorner)
{
    this->identifier = mixingOperation->mixerIdentifer;
    this->outputIdentifier = mixingOperation->outputIdentifier;
    this->outputType = mixingOperation->outputType;
    this->upperLeftCorner = upperLeftCorner;
    this->lowerRightCorner = upperLeftCorner + Direction(mixingOperation->rows - 1, mixingOperation->columns - 1);
    this->outputPosition = inputPosition;
    this->remainingTime = mixingOperation->duration;
}

Point Mixer::getUpperleftCorner()
{
    return this->upperLeftCorner;
}

Point Mixer::getLowerRightCorner()
{
    return this->lowerRightCorner;
}

int find(int x, int* a, int n) {
    for (int i = 0; i < n; i++) {
        if (x == a[i]) {
            return i;
        }
    }
    assert(false);
}

bool Mixer::overlap(Mixer* mixer)
{
    Line line[5];
    int r[5];
    r[0] = this->upperLeftCorner.r;
    r[1] = this->lowerRightCorner.r;
    r[2] = mixer->upperLeftCorner.r;
    r[3] = mixer->lowerRightCorner.r;
    sort(r, r + 4);
    int tot = 1;
    for (int i = 1; i < 4; i++) {
        if (r[i] != r[i-1]) {
            r[tot++] = r[i];
        }
    }
    getLine(this->upperLeftCorner, this->lowerRightCorner, line);
    getLine(mixer->upperLeftCorner, mixer->lowerRightCorner, line + 2);
    sort(line, line + 4);
    for (int i = 0; i < 4; i++) {
        line[i].r1 = find(line[i].r1, r, tot);
        line[i].r2 = find(line[i].r2, r, tot);
    }
    int vis[4];
    for (int i = 0; i < tot; i++) {
        vis[i] = 0;
    }
    for (int i = 0; i < 4; i++) {
        line[i].r1 = find(line[i].r1, r, tot);
        line[i].r2 = find(line[i].r2, r, tot);
        assert(line[i].r1 <= line[i].r2);
        for (int j = line[i].r1; j <= line[i].r2; j++) {
            if (vis[j] > 0) return true;
            vis[j] += line[i].type;
        }
    }
    for (int i = 0; i < tot; i++) {
        assert(vis[i] == 0);
    }
    return false;
}

int Mixer::getIdentifier()
{
    return this->identifier;
}

bool Mixer::inside(Point position)
{
    return this->upperLeftCorner.r <= position.r && this->upperLeftCorner.c <= position.c &&
            position.r <= this->lowerRightCorner.r && position.c <= this->lowerRightCorner.c;
}

Mixer::Mixer(Mixer* precursor)
{
    this->identifier = precursor->identifier;
    this->outputType = precursor->outputType;
    this->outputIdentifier = precursor->outputIdentifier;
    this->upperLeftCorner = precursor->upperLeftCorner;
    this->lowerRightCorner = precursor->lowerRightCorner;
    this->outputPosition = precursor->outputPosition;
    this->remainingTime = precursor->remainingTime - 1;
}

ULL Mixer::hash()
{
    static ULL hashBase = 821395121354ull;
    ULL ret = this->identifier;
    ret = grid->getPointIdentifier(this->upperLeftCorner) + hashBase * ret;
    ret = grid->getPointIdentifier(this->outputPosition) + hashBase * ret;
    ret = this->remainingTime + hashBase * ret;
    return ret;
}

int Mixer::getOutputIdentifier()
{
    return this->outputIdentifier;
}

int Mixer::getOutputType()
{
    return this->outputType;
}

Point Mixer::getOutputPosition()
{
    return this->outputPosition;
}

int Mixer::getRemainingTime()
{
    return this->remainingTime;
}

int Mixer::estimatedTime()
{
    int ret = this->remainingTime;
    if (detection[this->outputIdentifier]->duration != 0) {
        ret += manDis(this->outputPosition, detectorPosition[this->outputType])
            + detection[this->outputIdentifier]->duration;
    }
    return ret + leastTime[this->outputIdentifier];
}