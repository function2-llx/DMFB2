#include "IDMFB.h"
#include <string>
#include <chrono>

using namespace std;

struct Data {
    std::string name;
    int lines, time, steps;
};

Data test(string name) {
    Data data;
    data.name = name;
    data.lines = 0;
    auto f = fopen(name.c_str(), "r");
    if (f == nullptr) {
        throw "cannot open file: " + name;
    }
    char buf[1024];
    // int lines = 0;
    while (fgets(buf, sizeof(buf), f)) {
        data.lines++;
    }
    auto a = chrono::steady_clock::now();
    data.steps = IDMFB::get_steps(name, 15, 15, 500);
    auto b = chrono::steady_clock::now() - a;
    using typeb = decltype(b);
    data.time = chrono::duration_cast<chrono::milliseconds, typeb::rep, typeb::period>(b).count();
    return data;
}

int main() {
    vector<string> tests = {
        "material/Assays/B1/MixSplit/PCR.txt",
        "material/Assays/B2/InVitro_Ex1_2s_2r.txt",
        "material/Assays/B2/InVitro_Ex2_2s_3r.txt",
        "material/Assays/B2/InVitro_Ex3_3s_3r.txt",
        "material/Assays/B2/InVitro_Ex4_3s_4r.txt",
        "material/Assays/B2/InVitro_Ex5_4s_4r.txt",
    };
    auto f = fopen("test.txt", "w");
    for (auto name: tests) {
        auto data = test(name);
        fprintf(f, "%s , lines: %d steps: %d times: %d\n", data.name.c_str(), data.lines, data.steps, data.time);
        fflush(f);
    }
    return 0;
}