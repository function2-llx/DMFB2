#include <set>
#include <map>
#include <cassert>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "Global.h"
#include "entities/Dispenser.h"
#include "grid/Grid.h"
#include "core/State.h"
#include "core/DMFB.h"
#include "useless/PlaceState.h"
#include "placement/random_placing_strategy.h"
#include "math_models/sequencing_graph.h"

// using namespace std;

struct Node {
	int id, type;
	Node* fa;
	Node* ch[2];
	Node() {
		fa = nullptr;
		ch[0] = ch[1] = nullptr;
	}
	inline void insertChild(Node* child)
	{
		if (this->ch[0] == nullptr) {
			this->ch[0] = child;
		} else {
			assert(this->ch[1] == nullptr);
			this->ch[1] = child;
		}
	}
};

int DMFB::getDropletNumber() const { return this->nDroplets; }

static Node** node;

DMFB::DMFB()
{
	// this->typeMap.clear();
	this->nTypes = 0;
	this->nDispensers = 0;
}

DMFB::~DMFB()
{
	for (int i = 0; i < 4; i++)
		delete []this->boundary_record[i];

	for (int i = 0; i < this->rows; i++)
		delete []this->detector_record[i];

	delete []this->detector_record;

    for (auto sink: sinks)
        delete sink;

    for (auto dispenser: dispensers)
        delete dispenser;

    // for (auto detector: detectors)
    //     delete detector;
    delete detector;
}


static void dfs_least(Node* cur, std::vector<int>& least_time, const std::vector<DropletData>& droplet_data)
{
	if (cur == nullptr) 
		return;
	if (cur->fa != nullptr) 
		least_time[cur->id] += least_time[cur->fa->id] + 
            droplet_data[cur->fa->id].mixingTime + 
            droplet_data[cur->fa->id].detectingTime;
	for (int i = 0; i < 2; i++)
		dfs_least(cur->ch[i], least_time, droplet_data);
}

// std::vector<int> type;

void DMFB::load_sequencing_graph(const std::string& path)
{
	using namespace Global;

	// std::ifstream is(path + "SequencingGraph.txt");
	// assert(is.is_open());
    // SequencingGraph graph;
    graph.load_from_file((path + "SequencingGraph.txt").c_str());
    this->droplet_data = graph.get_droplet_data();

	// is >> this->nDroplets;
    this->nDroplets = droplet_data.size();
	node = new Node*[this->nDroplets];
	to_mix.resize(nDroplets, 0);
	mixing_result.resize(nDroplets, std::vector<int>(nDroplets, -1));
	for (int i = 0; i < this->nDroplets; i++)
		node[i] = new Node();
	for (int i = 0; i < this->nDroplets; i++) {
		node[i]->id = i;
        node[i]->type = droplet_data[i].type;

        int fa_id = droplet_data[i].fa_id;
        if (droplet_data[i].fa_id != -1) {
            node[i]->fa = node[fa_id];
            node[fa_id]->insertChild(node[i]);
        }

	}
    
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->fa == nullptr)
			to_mix[i] = false;
		else
            to_mix[i] = true;
	}
	for (int i = 0 ; i < this->nDroplets; i++) {
		if (node[i]->ch[0] == nullptr) {
			assert(droplet_data[i].mixingTime  == 0);
            dispense_id.push_back(i);
			assert(node[i]->ch[1] == nullptr);
            if (node[i]->type >= this->nTypes)
                this->nTypes = node[i]->type + 1;
		} 
	}

	this->nDispensers = this->nTypes;
	for (int i = 0; i < this->nDroplets; i++) {
		if (node[i]->fa == nullptr) {
			to_mix[i] = false;
            assert(droplet_data[i].output_sink != -1);
        }
		else 
            to_mix[i] = true;
		if (node[i]->ch[0] == nullptr) {
			assert(node[i]->ch[0] == nullptr);
		} else {
			Node *ch1 = node[i]->ch[0], *ch2 = node[i]->ch[1];
			assert(ch2 != nullptr);
            droplet_data[ch1->id].peer_id = ch2->id;
            droplet_data[ch2->id].peer_id = ch1->id;
			mixing_result[ch1->id][ch2->id] = mixing_result[ch2->id][ch1->id] = i;
		}
	}

    for (int i = 0; i < mixing_result.size(); i++)
        for (int j = 0; j < mixing_result[i].size(); j++) {
            std::cerr << "mixing result: " << i << '+' << j << '=' << mixing_result[i][j] << std::endl;
        }

    least_time.resize(nDroplets, 0);

	for (int i = 0; i < this->nDroplets; i++) 
		if (node[i]->fa == nullptr) 
			::dfs_least(node[i], this->least_time, this->droplet_data);

    this->nSinks = 0;
    for (auto data: droplet_data)
        if (data.output_sink >= this->nSinks)
            this->nSinks = data.output_sink + 1;
    assert(this->nSinks == 1);

	std::cerr << "sequencing graph loaded" << std::endl;
}

void DMFB::loadDesignObejective(const std::string& path)
{
    using namespace std;
    ifstream is(path + "DesignObjective.txt");
	assert(is.is_open());

	is >> this->rows >> this->columns;
	this->detector_record = new int*[this->rows];
	for (int i = 0; i < this->rows; i++) {
		this->detector_record[i] = new int[this->columns];
	}
	grid = new Grid(this->rows, this->columns);
	grid->build();
    cerr << this->nTypes << endl;
	assert(this->nTypes <= grid->area());
	for (int i = 0; i < 4; i++) {
		this->boundary_record[i] = new int[grid->boundarySize[i]];
	}
    auto set_disable = [&is] () {
        int r, c;
        vector<Point> disable_pos;
        while (is >> r >> c)
            disable_pos.push_back(Point(r - 1, c - 1));
        
        grid->disable(disable_pos);
    };

    set_disable();
    
	cerr << "design objective loaded" << endl;
}

static const State* ret;
static int* curBoundary[4];
static int** curDetector;
static int stepLowerBound, stepUpperBound;
// static int retStep;
static int target;
// static int curDispenserCount, curSinkCount, curDetectorCount;


bool DMFB::dfs(const State* currentState)
{
    using namespace std;

	bool flag = false;
	if (currentState->isEndState()) {
        flag = true;
        for (int k = 0; k < 4; k++) {
            for (int i = 0; i < grid->boundarySize[k]; i++) {
                this->boundary_record[k][i] = curBoundary[k][i];
            }
        }
        for (int i = 0; i < this->rows; i++) {
            for (int j = 0; j < this->columns; j++) {
                this->detector_record[i][j] = curDetector[i][j];
            }
        }

        if (ret != nullptr) {
            ret->clean();
        }
        assert(system("mkdir -p output") == 0);
        char st[100];
        sprintf(st, "output/%d.out", currentState->step);
        ofstream os(st);
        this->print_placement(os);
        ret = currentState;
        ret->printSolution(os);
        os << "time: " << (clock() - Global::start_time) / 1e6 << "s" << endl;
        cerr << "solution of " << ret->step;
        if (ret->step == 1) {
            cerr << " step";
        } else {
            cerr << " steps";
        }
        cerr << " found" << endl;
        cerr << "time: " << (clock() - Global::start_time) / 1e6 << "s" << endl;
        target = ret->step - 1;
    } else {
		if (currentState->step + currentState->estimationTime() <= stepUpperBound) {
			vector<const State*> successors = currentState->get_successors();
			sort(successors.begin(), successors.end());
			for (auto successor: successors) {
				if (dfs(successor)) {
					flag = true;
					break;
				}
			}
		}
	}
	if (!flag) delete currentState;
	return flag;
}

// void DMFB::placeDetector(int detectorCount)
// {
// 	static int cnt = 0;
// 	if (detectorCount == this->nTypes) {
// 		// cerr << cnt++ << endl;
// 		if (!placeState->addDetector(detectorCount, this->detectors))
// 			return ;
// 		for (stepUpperBound = stepLowerBound; stepUpperBound <= target; stepUpperBound++) {
// 			hashSet.clear();
//             // cerr << stepUpperBound << endl;
// 			State* init = new State;
// 			hashSet.insert(init->hash());
// 			if (dfs(init)) {
// 				assert(ret != nullptr);
// 				break;
// 			}
// 		}
// 	} else {
// 		for (int i = 0; i < grid->getRows(); i++) {
// 			for (int j = 0; j < grid->getColumns(); j++) {
// 				Point position = Point(i, j);
// 				// Detector* detector = new Detector(detectorCount, position);
//                 Detector *detector = detectors[detectorCount];
//                 detector->set_pos(position);
// 				// this->detectors[detectorCount] = detector;
// 				if (grid->placeDetector(detector, position)) {
// 					// detectorPosition[detectorCount] = position;
// 					curDetector[i][j] = detectorCount;
// 					this->placeDetector(detectorCount + 1);
// 					curDetector[i][j] = -1;
// 					grid->removeDetector(position);
// 				}
// 				// delete detector;
// 				if (target < stepLowerBound) return;
// 			}
// 		}
// 	}
// }

// void DMFB::placeSink(int sinkCount)
// {
// 	if (sinkCount == this->nSinks) {
// 		if (placeState->addSink(sinkCount, sinks)) {
// 			placeState->clearDetector();
// 			this->placeDetector(0);
// 		}
// 	} else {
// 		for (int k = 0; k < 4; k++) {
// 			for (int i = 0; i < grid->boundarySize[k]; i++) {
// 				if (curBoundary[k][i] == -1) {
// 					Point position = grid->boundaryPosition(i, k);
// 					Sink* sink = sinks[sinkCount];
// 					sink->set_pos(position);
// 					if (grid->placeSink(sink, position)) {
// 						curBoundary[k][i] = this->nTypes;
// 						this->placeSink(sinkCount + 1);
// 						grid->removeSink(position);
// 						curBoundary[k][i] = -1;
// 					}
// 					// delete sink;
// 					if (target < stepLowerBound) return;
// 				}
// 			}
// 		}
// 	}
// }


// void DMFB::placeDispenser(int dispenserCount)
// {
// 	if (dispenserCount == this->nDispensers) {
//         // cerr << "test" << endl;
// 		for (int i = 0; i < this->nDispensers; i++) {
// 			assert(grid->inside(dispensers[i]->getPosition()));
// 		}
// 		if (placeState->addDispenser(dispenserCount, dispensers)) {
// 			placeState->clearSink();
// 	    	this->placeSink(0);
// 		}
// 	} else {
// 		for (int k = 0; k < 4; k++) {
// 			for (int i = 0; i < grid->boundarySize[k]; i++) {
// 				if (curBoundary[k][i] == -1) {
// 					Point position = grid->boundaryPosition(i, k);
// 					assert(grid->inside(position));
// 					dispensers[dispenserCount]->setPosition(position);
// 					curBoundary[k][i] = dispenserCount;
// 					bool flag = true;
// 					if (flag) this->placeDispenser(dispenserCount + 1);
// 					curBoundary[k][i] = -1;
// 					if (target < stepLowerBound) return;
// 				}
// 			}
// 		}
// 	}
// }

// void DMFB::solve_placement_undetermined()
// {
// 	target = 100;
// 	curDetector = new int*[grid->getRows()];
// 	for (int i = 0; i < grid->getRows(); i++) {
// 		curDetector[i] = new int[grid->getColumns()];
// 		for (int j = 0; j < grid->getColumns(); j++) {
// 			curDetector[i][j] = -1;
// 		}
// 	}
// 	for (int i = 0; i < 4; i++) {
// 		curBoundary[i] = new int[grid->boundarySize[i]];
// 		for (int j = 0; j < grid->boundarySize[i]; j++) {
// 			curBoundary[i][j] = -1;
// 		}
// 	}
// 	assert(system("rm -f output/*") == 0);


// 	placeState = new PlaceState;
// 	placeState->set(grid->getRows(), grid->getColumns());
// 	placeState->clearDispenser();
// 	ret = nullptr;

// 	this->placeDispenser(0);

// 	for (int i = 0; i < 4; i++) {
// 		delete []curBoundary[i];
// 	}
// 	for (int i = 0; i < this->rows; i++) {
// 		delete []curDetector[i];
// 	}

// 	delete []curDetector;    
// 	delete placeState;
// }

void DMFB::print(std::ostream& os, int x)
{
    if (x == -2)
        os << "X ";
    else if (x == -1) {
		os << "N ";
	} else if (x == this->nTypes) {
		os << "S ";
	} else {
		// assert(0 <= x && x < this->nTypes);   // why <= 9?
		os << "D ";
	}
}

void DMFB::print_placement(std::ostream& os)
{
    using namespace std;

	os << "placement: " << endl;
	os << "  |";
	for (int j = 0; j < this->columns; j++) {
		this->print(os, this->boundary_record[(int)Grid::OuterPos::UP][j]);
		os << "|";
	}
	os << endl;
	for (int i = 0; i < this->rows; i++) {
		this->print(os, this->boundary_record[(int)Grid::OuterPos::LEFT][i]);
		os << "|";
		for (int j = 0; j < this->columns; j++) {
            if (grid->pos_available(Point(i, j)))
                this->print(os, this->detector_record[i][j]);
            else 
                this->print(os, -2);
			os<<"|";
		}
		this->print(os, this->boundary_record[(int)Grid::OuterPos::RIGHT][i]);
		os << endl;
	}
	os << "  |";
	for (int j = 0; j < this->columns; j++) {
		this->print(os, this->boundary_record[(int)Grid::OuterPos::DOWN][j]);
		os << "|";
	}
	os << endl;
}

//free states in queue that not in route
static void clear(std::vector<const State*> queue, std::vector<const State*> route)
{
	auto it = route.begin();
    int cnt = 0;
	for (auto state: queue) {
		if (state == *it)
			it++;
		else
			delete state;
	}

    assert(it + 1 == route.end());
}

#include <unordered_set>

std::vector<const State*> DMFB::get_route_bfs(const State* state) const
{
    using namespace std;

	std::unordered_set<State> state_set;
	if (state->isEndState())
		return std::vector<const State*>(1, state);	//already end state

	std::vector<const State*> que;
 	que.push_back(state);
	int head = 0, tail = 1;
	while (head < tail) {
		auto cur = que[head++];
        cerr << cur->step << ' ' << cur->getDroplets().size() << ' ' << que.size() << endl;
		for (auto suc: cur->get_successors()) {
			if (suc->isEndState()) {
				auto route = State::get_whole_route(state, suc);
				::clear(que, route);
				return route;
			} else if (!state_set.count(*suc)) {
				state_set.insert(*suc);
				que.push_back(suc);
                tail++;
			} else
                delete suc;
		}
	}

	return vector<const State*>();	//	no solution
}

const State* DMFB::dfs(const State* state, int upper_bound, std::unordered_set<State>& set) const
{
    // std::cerr << *state << std::endl;
    using namespace std;
    const State *ret = nullptr;

	if (state->isEndState())
        ret = state;
    else {
        assert(state != nullptr);
		if (state->step + state->estimationTime() <= upper_bound) {
			std::vector<const State*> successors = state->get_successors();
			sort(successors.begin(), successors.end(), [] (const State* a, const State* b) {
                return a->estimationTime() < b->estimationTime();
            });

			for (auto successor: successors) {
                if (ret != nullptr) // already found a solution
                    delete successor;
                else {
                    auto it = set.find(*successor);
                    if (it == set.end() || it->step > successor->step) {
                        if (it != set.end())
                            set.erase(it);
                        set.insert(*successor);
                        auto cur = dfs(successor, upper_bound, set);
                        if (cur != nullptr)
                            ret = cur;
                        else
                            delete successor;
                    } else
                        delete successor;
                }
			}
		}
	}

    return ret;
}

std::vector<const State*> DMFB::get_route_dfs(const State* state, int lim) const
{
    using namespace std;

    if (state->isEndState())
        return {state};

    int lower_bound = state->estimationTime();
    for (int upper_bound = lower_bound; upper_bound <= lim; upper_bound++) {
        cerr << upper_bound << endl;
        std::unordered_set<State> set;
        
        auto cur = dfs(state, upper_bound, set);
        if (cur != nullptr)
            return State::get_whole_route(state, cur);
    }

    return {};  // no solution
}

void DMFB::set_placement(const Placement& placement)
{
    this->placement = placement;
    using OuterPos = Grid::OuterPos;

    for (int i = 0; i < 4; i++) {
        Grid::OuterPos pos = static_cast<OuterPos>(i);
        int size;
        if (pos == OuterPos::LEFT || pos == OuterPos::RIGHT)
            size = rows;
        else size = columns;

        for (int j = 0; j < size; j++)
            boundary_record[i][j] = -1;
    }

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            detector_record[i][j] = -1;

    detector_record[placement.detector_pos.second.r][placement.detector_pos.second.c] = 0;

    // for (auto detector_pos: placement.detector_positions) {
    //     auto pos = detector_pos.second;
    //     auto detector = detector_pos.first;
    //     detector_record[pos.r][pos.c] = 1;
    //     // detector->get_type();
    // }

    for (auto dispenser_pos: placement.dispenser_positions) {
        auto dispenser = dispenser_pos.first;
        auto outer_id = grid->get_outer_id(dispenser_pos.second);
        boundary_record[(int)outer_id.first][outer_id.second] = dispenser->get_type();
    }

    for (auto sink_pos: placement.sink_positions) {
        auto sink = sink_pos.first;
        auto outer_id = grid->get_outer_id(sink_pos.second);
        boundary_record[(int)outer_id.first][outer_id.second] = nTypes;
    }
}

bool DMFB::place_entities()
{
    PlacingStrategy *strategy = new RandomPlacingStrategy(2333);
    Placement placement = strategy->get_placement(dispensers, sinks, detector, rows, columns);
    set_placement(placement);
    std::ofstream os("placement.txt");
    print_placement(os);

    std::cerr << "placement setted" << std::endl;
    
    for (auto dispenser_pos: placement.dispenser_positions)
        dispenser_pos.first->setPosition(grid->get_target_pos(dispenser_pos.second));
    
    placement.detector_pos.first->set_pos(placement.detector_pos.second);
    // for (auto detector_pos: placement.detector_positions)   
    //     detector_pos.first->set_pos(detector_pos.second);

    for (auto sink_pos: placement.sink_positions)
        sink_pos.first->set_pos(grid->get_target_pos(sink_pos.second));

    grid->set_placement(placement);
    delete strategy;

    return true;
}

void DMFB::init(const std::string& path)
{
    DMFBsolver->load_sequencing_graph(path);
	// DMFBsolver->loadModuleLibrary();
	DMFBsolver->loadDesignObejective(path);

    sinks.reserve(nSinks);
    for (int i = 0; i < nSinks; i++)
        sinks.push_back(new Sink(i));

    dispensers.reserve(nDispensers);
    for (int i = 0; i < nDispensers; i++)
        dispensers.push_back(new Dispenser(i));

    detector = new Detector;
}

std::vector<const State*> DMFB::get_route(const State* state) const { return this->get_route_dfs(state); }

void DMFB::solve_placement_determined()
{
    this->declare();
    this->place_entities();
    const State* init_state = new State();
    auto route = this->get_route(init_state);

    std::cerr << "route size: " << route.size() << std::endl;

    auto sequences = DMFBsolver->get_move_sequences(route);
    for (auto sequence: sequences) {
        for (auto pos: sequence) {
            std::cout << pos.r + 1 << ' ' << pos.c + 1<< ' ';
        }
        std::cout << std::endl;
    }

    for (auto state: route) {
        // state->allPrint(std::cout);
        delete state;
    }
}

std::vector<std::vector<Point> > DMFB::get_move_sequences(const std::vector<const State*>& route) const
{
    std::vector<std::vector<Point> > ret(this->nDroplets);
    for (auto state: route) {
        for (auto droplet: state->getDroplets()) {
            if (droplet->is_dispensed())
                ret[droplet->get_id()].push_back(droplet->get_pos());
            else {
                for (auto dispenser_pos: this->placement.dispenser_positions) {
                    if (dispenser_pos.first->get_type() == droplet->getType())
                        ret[droplet->get_id()].push_back(dispenser_pos.second);
                }
                // ret[droplet->get_id()].push_back(this->get_dispenser(droplet->get_id())->getPosition());
            }
        }
    }

    return ret;
}

DMFB *DMFBsolver;