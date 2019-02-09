#pragma once

#include <vector>
#include "Path.h"
#include "State.h"
#include <map>

class FlowNetwork {
private:
	struct Edge {
		int u, v, flow, cost;
		Edge *rev, *next;
		Edge(int u, int v, int flow, int cost) : u(u), v(v), flow(flow), cost(cost) {}
	};
	std::vector<const Edge*> edges;
	Edge** head;
	int source, sink;
	int step;
public:
	FlowNetwork(const State* state, int** bondary);
	~FlowNetwork();
	void addEdge(int u, int v, int flow, int cost);
	std::vector<Path> solve();
};