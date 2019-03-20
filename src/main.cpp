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

void solve_all()
{
	Global::start_time = clock();
	// DMFBsolver = new DMFB();
    DMFBsolver = new GreedySolver;
    // DMFBsolver = new BfsSolver();
    // DMFBsolver = new GreedyBfsSolver;
	
    DMFBsolver->init();
	// DMFBsolver->solve_placement_undetermined();
    DMFBsolver->solve_placement_determined();

	cerr << "end" << endl;
	delete DMFBsolver;
}

int main()
{
    char * const p = "23333";


    // solve_all();
    // const char *s[] = {
    //     "233",
    //     "2444"
    // };
    // cout << s[1] << endl;

    char buf[1000];
    // fscanf(stdin, "DagName (%[^)])", buf);
    // cerr << fscanf(stdin, "Node") << endl;
    // fscanf(stdin, "%*[2]%[^\n]", buf);
    puts(buf);

    // // scanf("233 test (%[^)]", buf);
    // fgets(buf, 4, stdin);
    // puts(buf);
    // printf("%lu\n", strlen(buf));
    // scanf("%[^23]s", buf);
    // puts(buf);
	return 0;
}