# DMFB
## 简介

本项目为生物微流控芯片自动路径规划程序，基于一种自行设计的算法，可以做到对输入的时序图（Sequencing Graph）进行路径规划并打印。

项目的 github 仓库地址[在此](https://github.com/function2-llx/DMFB2)。

## 使用方法

### 构建工程

使用 cmake 构建工程。

```bash
mkdir build
cd build
cmake ..
make
```

生成的可执行文件为 `bin/DMFB`，同时也会生成动态链接库 `lib/libi_dmfb.so`，可在 `include/IDMFB.h` 中添加其它接口。

### 运行方法

运行命令为：

```bash
<exec> <input> [<n>] [<m>]
```

其中 `input` 参数为测试的输入文件，`n`，`m`  为网格大小，默认为 15。

### 程序输出说明

程序运行后很快会生成 `placement.txt`，为各个部件（dispenser,detector,sink 等）的摆放。

运行过程中会打印出当前的局面，包含当前的状态的各种信息，以及各个液滴的信息，例如：

```
=================State=================
step 168
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   D13 N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   D12 N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   D10 N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   
N   N   N   D14 N   N   N   N   N   N   N   N   N   N   N   
N   N   N   N   N   N   N   N   N   N   N   N   N   N   N   

droplet state: 

droplet
identifier: 8
type: 10
position: (9, 2)
detecting state: has been detected

droplet
identifier: 14
type: 12
position: (6, 7)
detecting state: has been detected

droplet
identifier: 17
type: 13
position: (3, 1)
detecting state: has been detected

droplet
identifier: 20
type: 14
position: (13, 3)
detecting state: has been detected

next min: 0
current step: 168
board size: 4
estimated eventually step: 183
======================================
```

当路径规划完成后，会打印出所有液滴的的移动路径（move sequence）。

## 实现简述

各个部件的摆放的最优化不是程序的目标。程序运行后，首先会按照某种算法（目前是一种随机算法）生成一个摆放，然后开始路径规划。

实现的主要想法是对于一个时序图求出一个可行（尽量优的）解，而非在所有可行解中寻找一个最优解。于是实现方法是：对于每个状态，在其后继状态中选择一个最优的。主要方法有两个：

- 选择进行了有效操作的状态（有效操作指合并、检查、dump 等），或可能更快进行有效操作的后继状态；
- 对每个状态估计一个剩余时间

经过测试，第一种方法是最有效的。目前可以在较快的时间内（通常不超过半分钟）通过所有不含分裂（split）操作的测例。

## 测试结果

