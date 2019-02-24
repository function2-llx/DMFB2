#include <cassert>
#include <iostream>
#include <fstream>
#include "core/State.h"
#include "core/DMFB.h"
#include "Global.h"
#include "core/greedy_solver.h"

using namespace std;

int main()
{
	Global::start_time = clock();
	// DMFBsolver = new DMFB();
    DMFBsolver = new GreedySolver;
	
    DMFBsolver->init();
	// DMFBsolver->solve_placement_undetermined();
    DMFBsolver->solve_placement_determined();

	cerr << "end" << endl;
	delete DMFBsolver;
	return 0;
}