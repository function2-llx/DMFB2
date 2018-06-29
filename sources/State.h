#ifndef STATE_H
#define STATE_H

#include <set>
#include <vector>
#include "Droplet.h"

typedef unsigned long long ULL;

class State {
    friend std::ostream& operator << (std::ostream&, const State&);
private:
    std::vector<const Droplet*> droplets;
    int estimation;
    
    State& operator = (const State& state);
    State(const State&);
    State(const State* precursor);
    void clear();

    void addDroplet(const Droplet* droplet);

    void check() const;
    void dfsMove(unsigned int number) const;
    void pushDroplet(const Droplet& droplet, unsigned int number) const;
public:
    int step;
    const State* decision;

    State();
    ~State();
    
    ULL hash() const;
    void clean() const;

    bool isEndState() const;
    std::vector<const State*> getSuccessors() const;
    int estimationTime() const;
    bool operator < (const State&) const;

    void printSolution(std::ostream&) const;
    void textPrint(std::ostream&) const;
    void visualPrint(std::ostream&) const;
    void allPrint(std::ostream&) const;
};

#endif  //STATE_H