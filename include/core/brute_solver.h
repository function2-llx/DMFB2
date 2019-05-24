#include "core/DMFB.h"

class BruteSolver : public DMFB {
    std::vector<const State*> get_route(const State* state, int lim) const override;

    void declare() const override{
        using namespace std;
        cerr << "solve by brute force" << endl;
    }
};