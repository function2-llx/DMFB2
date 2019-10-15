#include "IDMFB.h"
#include "core/brute_solver.h"

using namespace IDMFB;

using namespace std;

int IDMFB::get_steps(std::string filename, int n, int m, int lim) {
    DMFBsolver = new BruteSolver;
    DMFBsolver->init(filename, n, m);
    // cerr << filename + "233" << endl;
    return DMFBsolver->solve_placement_determined(lim).size();
    delete DMFBsolver;
}

vector<MoveSequence> IDMFB::get_move_sequences(const string& filename, int n, int m, int lim) {
    DMFBsolver = new BruteSolver;
    return DMFBsolver->get_move_sequences(filename, n, m, lim);
    // DMFBsolver->init(filename);
}

ostream& operator << (ostream& os, const MoveSequence& seq) {
    os << "id: " << seq.droplet_id << endl;
    os << "start step: " << seq.t << endl;
    for (auto pos: seq.route) {
        os << pos << ' ';
    }
    os << endl;
    // os << seq.route << endl;
    return os;
}
