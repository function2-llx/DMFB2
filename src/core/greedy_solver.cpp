#include <algorithm>
#include "core/greedy_solver.h"
#include "core/greedy_state.h"


std::vector<const State*> GreedySolver::get_route(const State* init) const 
{
    init = new GreedyState(*init);

    auto ret = get_route_dfs(init);
    for (auto &state: ret) {
        auto tmp = state;
        state = new State(*state);
        delete tmp;
    }   //  replace greedy state to original state

    return ret;
}
