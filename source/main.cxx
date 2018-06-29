#include <cassert>
#include <iostream>
#include <fstream>
#include "State.h"
#include "DMFB.h"

using namespace std;

int main()
{
	DMFB* DFMBsolver = new DMFB();
	DFMBsolver->loadSequencingGraph();
	DFMBsolver->loadModuleLibrary();
	DFMBsolver->loadDesignObejective();
	DFMBsolver->solve();
	cerr << "end" << endl;
	delete DFMBsolver;
	return 0;
}