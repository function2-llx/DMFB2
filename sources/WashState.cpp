#include <cassert>
#include "WashState.h"
#include "WasherRouter.h"

using namespace std;

bool WashState::check() const
{
	for (auto flag: this->completed[this->step]) {
		if (!flag) {
			return false;
		}
	}
	return true;
}

static vector<const WashState*> successors;

void WashState::dfs(unsigned int number) const
{
	if (number == this->washers.size()) {
		
	} else {

	}
}

vector<const WashState*> WashState::getSuccessors() const
{

}