#include <vector>
#include "Grid.h"
#include "WasherRouter.h"

using namespace std;

WasherRouter::WasherRouter(const State* state)
{
	
}

bool WasherRouter::dfs(const WashState* state)
{
	if (state->isEndState()) {
		return true;
	} else {
		auto successors = state->getSuccessors();
		for (auto successor: successors) {
			if (this->dfs(successor)) {
				return true;
			}
		}
	}
	return false;
}

bool WasherRouter::Route()
{
	const WashState *init = new WashState();
	
}

bool WasherRouter::canReach(int time, Point position) const
{
	return reachable[time][position.r][position.c];
}

vector<Wash> WasherRouter::getWashes() const
{
	return this->washes;
}

int WasherRouter::getSteps() const
{
	return this->endState->step;
}

WasherRouter* washerRouter;