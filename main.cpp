#include <cstdlib>
#include <iostream>
#include "include/IDMFB.h"

using namespace std;

int main(int argc, char* argv[])
{
    // string path = "material/Assays/B1/MixSplit/PCR.txt";
    string path;
    int n = 15, m = 15;
    if (argc > 1) {
        path = argv[1];
        if (argc > 2) {
            n = atoi(argv[2]);
            if (argc > 3) {
                m = atoi(argv[3]);
            }
        }
    } else {
        fprintf(stderr, "usage: <exec> <input> [<n> <m>]\n");
        exit(1);
    }

    for (auto seq: IDMFB::get_move_sequences(path, n, m, 500)) {
        cout << seq << endl;
    }
    return 0;
}