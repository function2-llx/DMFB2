#ifndef STATE_H
#define STATE_H

#include <set>
#include <vector>
#include "Droplet.h"

typedef unsigned long long ULL;

class State {
    friend std::ostream& operator << (std::ostream&, State&);
private:
    std::vector<Dispensation*> dispensations;
    std::vector<Droplet*> droplets;
    std::vector<MixingOperation*> mixingOperations;
    std::vector<Mixer*> mixers;
    int estimation;
    
    State& operator = (const State& state);
    State(const State&);
    void clear();

    void addDispensation(Dispensation* dispensation);
    void removeDispensation();
    void addDroplet(Droplet* droplet);
    void addMixingOperation(MixingOperation* mixiongOperation);
    void removeMixingOperation();
    void addMixer(Mixer* mixer); 
    void removeMixer();

    bool canDump(Droplet* droplet);
    Droplet* findDroplet(Detection* detection);
    void check();
    void pushDroplet(std::vector<Droplet*>::iterator, Droplet* droplet);
    void pushDroplet(std::vector<Dispensation*>::iterator, Droplet* droplet);
    void dfsMove(std::vector<Droplet*>::iterator);
    void dfsDispense(std::vector<Dispensation*>::iterator);
    bool canPlace(Mixer* mixer);
    bool fluidicConstraints(Droplet*);
public:
    static State* initialState();

    int step;
    State* decision;

    State();
    ~State();
    
    ULL hash();
    bool isEndState();
    std::vector<State*> getSuccessors();
    void printSolution(std::ostream&);
    void clean();
    int estimatedTime();
    void textPrint(std::ostream&);
    void vitualPrint(std::ostream&);
    void allPrint(std::ostream&);
    bool operator < (const State&) const;
};

#endif  //STATE_H