#include <algorithm>
#include <unordered_set>
#include "core/brute_solver.h"

static const State* select(const State* state, std::unordered_set<State>& set)
{
    auto successors = state->get_successors();
    sort(successors.begin(), successors.end(), [](const State* a, const State* b) {
        return a->estimationTime() < b->estimationTime();
    });

    for (auto successor: successors) {
        if (successor->get_droplets_on_board_size() != state->get_droplets_on_board_size() && !set.count(*successor)) {
            // set.insert(*successor);
            return successor;
        }
    }

    for (auto successor: successors) {
        if (!set.count(*successor)) {
            // set.insert(*successor);
            return successor;
        }
    }

    if (!successors.empty())
        return successors.front();

    return nullptr;
}

std::vector<const State*> BruteSolver::get_route(const State* state) const
{
    using namespace std;
    unordered_set<State> set;
    std::vector<const State*> ret = {state};
    // std::unordered_set<State> set;
    // if (state->isEndState())
    //     return ret;
    for (auto cur = state;;) {
        set.insert(*cur);
        using namespace std;
        cerr << cur->step << endl;
        cerr << *cur << endl;
        if (cur->isEndState()) {
            // cerr << "end: " << cur->getDroplets().size() << *cur << endl;
            return ret;
        }
        auto successors = cur->get_successors();
        cur = ::select(cur, set);
        if (cur == nullptr)
            return {};

        ret.push_back(cur);        
    }
    // return ret;
    // return {};
}