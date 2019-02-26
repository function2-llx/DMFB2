#pragma once

#include <algorithm>
#include "State.h"

class GreedyState : public State {
  public:
    GreedyState(const State& state) : State(state) {}

    std::vector<const State*> get_successors() const override;
};