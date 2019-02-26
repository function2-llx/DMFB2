#pragma once

#include "core/DMFB.h"

class BfsSolver : public DMFB {
  protected:
    void declare() const override
    {
        std::cerr << "solve by bfs" << std::endl;
    }
  public:
    std::vector<const State*> get_route(const State* init) const;
};