#ifndef STATE_H
#define STATE_H

#include <set>
#include <vector>
#include "core/Map.h"
#include "droplet/Droplet.h"

typedef unsigned long long ULL;

class State {
  private:
    std::vector<const Droplet*> droplets;
    int estimation;
    
    State& operator = (const State& state);
    State(const State* precursor);

    void addDroplet(const Droplet* droplet);

    void dfsMove(unsigned int number) const;
    void pushDroplet(const Droplet& droplet, unsigned int number) const;
    void sort_droplets();
  public:
    int step;
    const State* decision;

    State();
    State(const State&);
    virtual ~State();

    std::vector<const Droplet*> getDroplets() const;
    
    ULL hash() const;
    void clean() const;

    bool isEndState() const;
    virtual std::vector<const State*> get_successors() const;
    int estimationTime() const;
    // bool operator < (const State&) const;

    void printSolution(std::ostream&) const;
    void textPrint(std::ostream&) const;
    void visualPrint(std::ostream&) const;
    void allPrint(std::ostream&) const;

    static std::vector<const State*> get_whole_route(const State* start, const State* end);
    
    friend std::ostream& operator << (std::ostream&, const State&);
    friend bool operator == (const State&, const State&);
};

namespace std {
    template<>
        struct hash<State> {
            size_t operator () (const State& state) const { return state.hash(); } 
        };
};

// extern Map<State, int> state_step;

#endif  //STATE_H