#include <vector>
#include "Grid.h"
#include "WasherRouter.h"

using namespace std;

WasherRouter::WasherRouter(const State* state)
{
	
}

bool WasherRouter::Route()
{
	const WashState *init = new WashState();

}

bool WasherRouter::canReach(int time, Point position) const
{
	return reachable[time][position.r][position.c];
}

vector<Wash> WasherRouter::getWash(int time) const
{
	return this->wash[time];
}