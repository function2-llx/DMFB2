#include "FlowNetwork.h"
#include "Grid.h"

using namespace std;

void search(const State* state, vector<const State*>& vecState)
{
	if (state->step == 0) {
		vecState.clear();
		return;
	}
	search(state->decision, vecState);
	vecState.push_back(state);
}

FlowNetwork::FlowNetwork(const State* state, int** bondary)
{
	this->edges.clear();
	this->step = state->step;
	int n = state->step * grid->area();
	this->source = n;
	this->sink = n + 1;
	n += 2;
	this->head = new Edge*[n];
	vector<const State*> vecState;
	search(state, vecState);
	bool record[state->step][grid->getRows()][grid->getColumns()];
	for (int i = 0; i < state->step; i++) {
		for (int j = 0; j < grid->getRows(); j++) {
			for (int k = 0; k < grid->getColumns(); k++) {
				record[i][j][k] = false;
			}
		}
	}
	for (int t = 0; t < state->step; t++) {
		auto droplets = vecState[t]->getDroplets();
		for (auto droplet: droplets) {
			auto position = droplet->getPosition();
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1 ;j++) {
					Point cur(position.r + i, position.c + j);
					if (grid->inside(cur)) {
						record[t][cur.r][cur.c] = true;
					}
				}
			}
		}
		for (int i = 0; i < grid->getRows(); i++) {
			for (int j = 0; j < grid->getColumns(); j++) {
				
			}
		}
	}

}

FlowNetwork::~FlowNetwork()
{
	for (auto edge: this->edges) {
		delete edge;
	}
	delete[] head;
}

void FlowNetwork::addEdge(int u, int v, int flow, int cost)
{
	Edge* e1 = new Edge(u, v, flow, cost);
	Edge* e2 = new Edge(v, u, 0, -cost);
	e1->rev = e2;
	e2->rev = e1;	
	this->edges.push_back(e1);
	this->edges.push_back(e2);
}

std::vector<Path> FlowNetwork::solve()
{
	
}