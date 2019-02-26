#include "core/bfs_solver.h"

std::vector<const State*> BfsSolver::get_route(const State* init) const { return get_route_bfs(init); }