#pragma once

#include "core/DMFB.h"

class GreedySolver : public DMFB {
  protected:
    void declare() const override
    {
        std::cerr << "solve by greedy dfs" << std::endl;
    }

  public:
    virtual std::vector<const State*> get_route(const State*) const override;
};