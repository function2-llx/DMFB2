#include <iostream>
#include "include/IDMFB.h"

using namespace std;

int main(int argc, char* argv[])
{
    string path = "material/Assays/B1/MixSplit/PCR.txt";
    int n = 21, m = 21;
    if (argc > 1) {
        path = argv[2];
        if (argc > 3) {
            n = atoi(argv[3]);
            m = atoi(argv[4]);
        }
    }


    for (auto seq: IDMFB::get_move_sequences(path, n, m, 300)) {
        cout << seq << endl;
    }
    return 0;
}