#pragma once

#include <set>
#include <map>
#include <cassert>
#include <unordered_set>
#include "core/State.h"
#include "placement/placement.h"


class Sink;
class Detector;

class DMFB {
  private:
    std::map<int, int> typeMap;
    int nDroplets, nSinks, nTypes, nDispensers;
    int rows, columns;
    int* boundary_record[4];
    int** detector_record;

    Placement placement;

    std::vector<std::vector<int> > mixing_result;
    std::vector<bool> to_mix;
    std::vector<DropletData> droplet_data;
    std::vector<int> least_time;
    std::vector<int> dispense_id;
    std::vector<int> real_type;

    std::vector<Dispenser*> dispensers;
    std::vector<Sink*> sinks;
    std::vector<Detector*> detectors;

    DMFB(const DMFB&);
    DMFB& operator = (const DMFB&);
    
    void print(std::ostream&, int);

    void placeSink(int sinkCount);
    void placeDispenser(int dispenserCount);
    void placeDetector(int detectorCount);
    bool dfs(const State*);
    const State* dfs(const State*, int upper_bound, std::unordered_set<State>&) const;
    
    bool place_entities(); //  return true if success

  protected:
    virtual void declare() const
    {
        std::cerr << "solve by typical dfs" << std::endl;
    }
  public:
    DMFB();
    ~DMFB();

    void loadSequencingGraph();
    void loadModuleLibrary();
    void loadDesignObejective();

    // DropletData get_mix_data(const Droplet*, const Droplet*);
    DropletData get_droplet_data(int id) const
    {
        assert(0 <= id && id < nDroplets);
        return droplet_data[id];
    }
    int get_mixing_result_id(int id_a, int id_b)
    {
        assert(0 <= id_a < nDroplets);
        assert(0 <= id_b < nDroplets);
        return mixing_result[id_a][id_b];
    }
    int get_mixing_result_id(const Droplet* a, const Droplet* b) const { return mixing_result[a->get_id()][b->get_id()]; }
    int get_least_time(const Droplet* droplet) const { return least_time[droplet->get_id()]; }
    bool is_to_mix(const Droplet* droplet) const { return to_mix[droplet->get_id()]; }
    std::vector<int> get_dispense_id() const { return dispense_id; }
    int get_real_type(int mapped_type) const
    {
        assert(0 <= mapped_type && mapped_type < real_type.size());
        return real_type[mapped_type];
    }

    int getDropletNumber() const;
    Dispenser* get_dispenser(int type) const
    {
        assert(0 <= type && type < nTypes);
        return dispensers[type];
    }

    Detector* get_detector(int type) const
    {
        assert(0 <= type && type < nTypes);
        return detectors[type];
    }

    void init();
    void solve_placement_undetermined();
    void solve_placement_determined();
    void print_placement(std::ostream&);
    void set_placement(const Placement& placement);

    virtual std::vector<const State*> get_route(const State*) const;
    std::vector<const State*> get_route_bfs(const State* state) const;
    std::vector<const State*> get_route_dfs(const State* state, int lim = 100) const;   //  at most lim steps
};

extern DMFB *DMFBsolver;
// extern std::vector<int> type;
