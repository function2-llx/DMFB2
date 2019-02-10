#pragma once

#include "core/State.h"
#include "useless/Wash.h"
#include "useless/WashState.h"

class WasherRouter {
private:
	std::vector<Wash> washes;
	bool ***reachable;	//reachable[t][r][c]表示在t时刻，格子(r,c)没有被规划好的路径占据
	int **boudary;
	const State *endState;
public:
	const WashState *result;

	~WasherRouter();
	WasherRouter(const State* endState, int** boudary);	//构造函数传入最后的状态，计算出所有的清洗任务装入wash中，同时计算计算各个时刻每个格子的reachalbe

	bool Route();
	bool dfs(const WashState*);
	bool canReach(int time, Point position) const;
	int getSteps() const;
	std::vector<Wash> getWashes() const;
};

extern WasherRouter* washerRouter;
