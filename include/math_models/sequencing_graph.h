#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include "droplet/DropletData.h"

// a tree
class SequencingGraph {
  private:
    struct Node {
        enum Type{
            DISPENSE = 0,
            MIX      = 1,
            DILUTE   = 2,
            SPLIT    = 3,
            HEAT     = 4,
            DETECT   = 5,
            OUTPUT   = 6,
            STORAGE  = 7,
            GENERAL  = 8
        };

        Type type;
        int drop_id, mixing_time, detecting_time, drop_type, sink_id;
        // std::string drop_type;

        Node *fa = nullptr;
        std::vector<Node*> sons;

        void insert(Node *son)
        {
            son->fa = this;
            this->sons.push_back(son);
        }
    };

    std::vector<Node*> nodes;
    const char *name;
    std::vector<DropletData> droplet_data;

  public:
    void load_from_file(const char*);
    using NodeType = Node::Type;

    const std::vector<DropletData>& get_droplet_data() const {return this->droplet_data; }
};