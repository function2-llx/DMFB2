#pragma once

#include "core/bfs_solver.h"

class GreedyBfsSolver : public BfsSolver {
  protected:
    void declare() const override
    {
        std::cerr << "solve by greedy bfs" << std::endl;
    }

  public:
    std::vector<const State*> get_route(const State* init) const override;
};