#include <vector>
#include "core/greedy_state.h"

std::vector<const State*> GreedyState::get_successors() const
{
    using namespace std;
    auto all_successors = State::get_successors();
    if (all_successors.empty())
        return {};

    int min_time = (*std::min_element(all_successors.begin(), all_successors.end(), [] (const State* a, const State* b) {
        return a->estimationTime() < b->estimationTime();
    }))->estimationTime();
    
    std::vector<const State*> ret;
    for (auto successor: all_successors) {
        if (successor->estimationTime() == min_time)
            ret.push_back(new GreedyState(*successor));

        delete successor;
    }
    
    return ret;
}