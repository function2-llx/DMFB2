#include "core/DMFB.h"
#include "core/brute_solver.h"

int get_steps(std::string filename, int n, int m, int lim) {
    DMFBsolver = new BruteSolver;
    return DMFBsolver->solve_placement_determined().size();
}