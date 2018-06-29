#ifndef STATE_H
#define STATE_H

#include <set>
#include <vector>
#include "Droplet.h"

typedef unsigned long long ULL;

class State {
    friend std::ostream& operator << (std::ostream&, State&);
private:
    std::vector<Droplet*> droplets;
    int estimation;
    
    State& operator = (const State& state);
    State(const State&);
    void clear();

    void addDroplet(Droplet* droplet);

    bool canDump(const Droplet* droplet) const;
    void check();
    void dfsMove(const std::vector<Droplet*>::iterator) const;
    void pushDroplet(const Droplet& droplet, const std::vector<Droplet*>::iterator) const;
    bool fluidicConstraints(Droplet*);
public:
    static State* initialState();

    int step;
    State* decision;

    State();
    ~State();
    
    ULL hash() const;
    bool isEndState();
    std::vector<State*> getSuccessors();
    void printSolution(std::ostream&);
    void clean();
    int estimatedTime();
    void textPrint(std::ostream&);
    void visualPrint(std::ostream&);
    void allPrint(std::ostream&);
    bool operator < (const State&) const;
};

#endif  //STATE_H