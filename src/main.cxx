#include <cassert>
#include <iostream>
#include <fstream>
#include "State.h"
#include "DMFB.h"
#include "Global.h"

using namespace std;

int main()
{
	Global::start = clock();
	DMFBsolver = new DMFB();
	DMFBsolver->loadSequencingGraph();
	DMFBsolver->loadModuleLibrary();
	DMFBsolver->loadDesignObejective();
	DMFBsolver->solve();
	cerr << "end" << endl;
	delete DMFBsolver;
	return 0;
}