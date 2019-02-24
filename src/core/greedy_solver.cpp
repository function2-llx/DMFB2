#include <algorithm>
#include "core/greedy_solver.h"

class GreedyState : public State {
  public:
    GreedyState(const State& state) : State(state) {}

    std::vector<const State*> get_successors() const override
    {
        using namespace std;
        auto all_successors = State::get_successors();
        if (all_successors.empty())
            return {};
        // cerr << all_successors.size() << endl;
        // return all_successors;
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

        // std::vector<const State*> successors;
        // for (auto successor: all_successors)
        //     if (successor->estimationTime() == min_time)
        //         successors.push_back(static_cast<const State*>(successor));
        //     else
        //         delete successor;

        // return successors;
        // return {static_cast<const GreedyState*>(*std::min_element(all_successors.begin(), all_successors.end(), [] (const State* a, const State* b)-> bool {
        //     return a->estimationTime() < b->estimationTime();
        // }))};

        // auto ret = new GreedyState(**std::min_element(all_successors.begin(), all_successors.end(), [] (const State* a, const State* b)-> bool {
        //     return a->estimationTime() < b->estimationTime();
        // }));

        // for (auto successor: all_successors)
        //     delete successor;

        // return {ret};
    }
};

std::vector<const State*> GreedySolver::get_route(const State* init) const 
{
    init = new GreedyState(*init);
    // std::vector<const State*> ret;
    // for (;;) {
    //     // std::cerr << *state << std::endl;
    //     ret.push_back(state);
    //     if (state->isEndState())
    //         return ret;

    //     state = state->get_successors().front();
    // }

    auto ret = get_route_dfs(init);
    for (auto &state: ret) {
        auto tmp = state;
        state = new State(*state);
        delete tmp;
    }   //  replace greedy state to original state

    return ret;
}
