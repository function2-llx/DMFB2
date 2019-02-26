#include <cassert>
#include <iostream>
#include <fstream>
#include "core/State.h"
#include "core/DMFB.h"
#include "Global.h"
#include "core/greedy_solver.h"
#include "core/bfs_solver.h"
#include "core/greedy_bfs_solver.h"

using namespace std;

void solve_all()
{
	Global::start_time = clock();
	// DMFBsolver = new DMFB();
    // DMFBsolver = new GreedySolver;
    // DMFBsolver = new BfsSolver();
    DMFBsolver = new GreedyBfsSolver;
	
    DMFBsolver->init();
	// DMFBsolver->solve_placement_undetermined();
    DMFBsolver->solve_placement_determined();

	cerr << "end" << endl;
	delete DMFBsolver;
}

int main()
{
    solve_all();
	return 0;
}