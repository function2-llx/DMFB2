#include <cassert>
#include <iostream>
#include <fstream>
#include "core/State.h"
#include "core/DMFB.h"
#include "Global.h"

using namespace std;

int main()
{
	Global::start_time = clock();
	DMFBsolver = new DMFB();
	
    DMFBsolver->init();
	// DMFBsolver->solve_placement_undetermined();
    DMFBsolver->solve_placement_determined();

	cerr << "end" << endl;
	delete DMFBsolver;
	return 0;
}