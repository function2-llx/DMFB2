#include <iostream>
#include "include/IDMFB.h"

using namespace std;

int main(int argc, char* argv[])
{
    string path = "material/Assays/B1/MixSplit/PCR.txt";
    int n = 10, m = 10;
    if (argc > 1) {
        path = argv[2];
        if (argc > 3) {
            n = atoi(argv[3]);
            m = atoi(argv[4]);
        }
    }


    for (auto seq: IDMFB::get_move_sequences(path, n, m)) {
        cout << seq << endl;
    }
    return 0;
}