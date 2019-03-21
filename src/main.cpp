#include <cassert>
#include <iostream>
#include <fstream>
#include <cstring>
#include "core/State.h"
#include "core/DMFB.h"
#include "Global.h"
#include "core/greedy_solver.h"
#include "core/bfs_solver.h"
#include "core/greedy_bfs_solver.h"

using namespace std;

void solve_all(const string& path)
{
	Global::start_time = clock();
	// DMFBsolver = new DMFB();
    DMFBsolver = new GreedySolver;
    // DMFBsolver = new BfsSolver();
    // DMFBsolver = new GreedyBfsSolver;
	
    DMFBsolver->init(path);
	// DMFBsolver->solve_placement_undetermined();
    DMFBsolver->solve_placement_determined();

	cerr << "end" << endl;
	delete DMFBsolver;
}

int main(int argc, char* argv[])
{
    string path = "./input/";
    if (argc > 1) {
        path = argv[1];
        if (path.back() != '/')
            path += "/";
    }

    solve_all(path);
	return 0;
}