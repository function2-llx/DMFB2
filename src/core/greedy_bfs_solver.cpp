#include "core/greedy_bfs_solver.h"
#include "core/greedy_state.h"

std::vector<const State*> GreedyBfsSolver::get_route(const State* init) const
{
    init = new GreedyState(*init);

    auto route = get_route_bfs(init);
    for (auto &state: route) {
        auto tmp = state;
        state = new State(*state);
        delete tmp;
    }

    return route;
}