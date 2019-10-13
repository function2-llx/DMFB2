import os, sys, time, ctypes
from sys import stderr

def test(filename):
    with open(filename, 'r') as f:
        lines = len(f.readlines())
    
    dmfb = ctypes.cdll.LoadLibrary('./lib/libi_dmfb.so')
    start = time.clock()
    steps = dmfb.get_steps(filename, 15, 15, 500)
    elapsed = time.clock() - start
    return lines, steps, elapsed


if __name__ == "__main__":
    test('material/Assays/B1/MixSplit/PCR.txt')
    for filename in '''material/Assays/B2/InVitro_Ex1_2s_2r.txt material/Assays/B2/InVitro_Ex2_2s_3r.txt material/Assays/B2/InVitro_Ex3_3s_3r.txt material/Assays/B2/InVitro_Ex4_3s_4r.txt material/Assays/B2/InVitro_Ex5_4s_4r.txt'''.split(' '):
        test(filename)