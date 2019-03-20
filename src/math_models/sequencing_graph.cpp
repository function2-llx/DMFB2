#include <cstdio>
#include <vector>
#include <cassert>
#include <unordered_map>
#include "math_models/sequencing_graph.h"
// #include "droplet/DropletData.h"

struct StringMap {
    std::unordered_map<std::string, int> map;
    std::vector<std::string> list;
    // int cnt;

    size_t size() { return list.size(); }

    int get_id(const std::string& s)
    {
        if (map.count(s))
            return map[s];
        int ret = map[s] = list.size();
        list.push_back(s);
        return ret;
    }

    std::string get_key(int id) 
    {
        assert(0 <= id && id < list.size());
        return list[id];
    }
};

struct MixTypeMap {
    std::unordered_map<std::pair<int, int>, int> map;

    std::vector<std::pair<int, int> > list;
    int pre_cnt;

    // int cnt;
    MixTypeMap(int pre_cnt) : pre_cnt(pre_cnt) {}

    int get_mix_type(int u, int v)
    {
        auto tmp = std::make_pair(u, v);
        if (map.count(tmp))
            return map[tmp] + pre_cnt;

        int ret = map[tmp] = list.size();
        list.push_back(tmp);
        return ret + pre_cnt;
    }
};

struct Edge {
    int u, v;

    Edge(int u, int v) : u(u), v(v) {}
};

using Type = SequencingGraph::NodeType;

static Type get_type(const char* s)
{
    static constexpr const char *types[] = {
        "DISPENSE",
        "MIX",
        "DILUTE",
        "SPLIT",
        "HEAT",
        "DETECT",
        "OUTPUT",
        "STORAGE",
        "GENERAL"
    };

    for (int i = 0; i < sizeof(types); i++) {
        if (strcmp(types[i], s) == 0)
            return static_cast<Type>(i);
    }

    throw "wrong type";
}

void SequencingGraph::load_from_file(const char* filename)
{
    // std::unordered_map<std::string, int> type_map;
    // int type_cnt = 0;
    StringMap type_map, sink_map;
    auto file = fopen(filename, "r");
    char buf[1024];
    fgets(buf, sizeof(buf), file);  //  the useless first line
    fscanf(file, "DagName (%[^)])\n", buf);
    std::vector<Node*> nodes;
    std::vector<Edge> edges;
    while (fgets(buf, sizeof(buf), file)) {
        char message_type[10];
        sscanf(buf, "%4[DENGO] (", message_type);
        if (strcmp(message_type, "NODE") == 0) {
            Node *u = new Node;
            nodes.push_back(u);
            int id;
                        
            char operation[10], drop_type[10], sink_name[10];
            sscanf(buf, "%d, %[^,], ", &id, operation);
            assert(id == nodes.size());
            switch (u->type = ::get_type(operation)) {
                case Node::DISPENSE:
                    sscanf(buf, "%[^,]", drop_type);
                    u->drop_type = type_map.get_id(drop_type);
                break;

                case Node::MIX:
                    sscanf(buf, "%*[^ ]%d", &u->mixing_time);
                break;

                case Node::DILUTE:
                    //  nothing to do
                break;

                case Node::SPLIT:
                    // nothing to do
                break;

                case Node::HEAT:
                    //  nothing to do
                break;

                case Node::DETECT:
                    sscanf(buf, "%*[^,] %d", &u->detecting_time);
                break;

                case Node::OUTPUT:
                    sscanf(buf, "%[^,]", sink_name);
                    u->sink_id = sink_map.get_id(sink_name);
                break;

                case Node::STORAGE:
                    //  nothing to do
                break;

                case Node::GENERAL:
                    //  nothing to do
                break;                
            }
        } else {
            int u, v;
            sscanf(buf, "%d, %d", &u, &v);
            edges.push_back(Edge(u - 1, v - 1));
        }
    }

    for (auto edge: edges)
        nodes[edge.v]->insert(nodes[edge.u]);

    int drop_cnt = 0;
    for (auto node: nodes) {
        if (node->type == Node::DISPENSE || node->type == Node::MIX || node->type == Node::DILUTE) {
            node->drop_id = drop_cnt++;
            for (auto u = node->fa;; u = u->fa) {
                if (u == nullptr || u->type == Node::MIX)
                    break;
                
                u->drop_id = node->drop_id;
            }
        }
    }

    MixTypeMap mix_map(type_map.size());
    droplet_data.resize(drop_cnt);
    for (auto node: nodes) {
        auto &data = droplet_data[node->drop_id];
        if (node->fa != nullptr && node->fa->type == Node::MIX)
            data.fa_id = node->fa->drop_id;

        switch (node->type) {
            case Node::DISPENSE:
                data.id = node->drop_id;
                data.type = node->drop_type;
            break;

            case Node::MIX: 
                data.id = node->drop_id;
                data.mixingTime = node->mixing_time;
                assert(node->sons.size() == 2);
                // auto u = node->sons.front(), v = node->sons.back();
                data.type = mix_map.get_mix_type(node->sons.front()->drop_type, node->sons.back()->drop_type);
            break;

            case Node::DILUTE:
                //  nothing to do
            break;

            case Node::SPLIT:
                // nothing to do
            break;

            case Node::HEAT:
                //  nothing to do
            break;

            case Node::DETECT:
                data.detectingTime = node->detecting_time;
            break;

            case Node::OUTPUT:
                data.output_sink = node->sink_id;
            break;

            case Node::STORAGE:
                //  nothing to do
            break;

            case Node::GENERAL:
                //  nothing to do
            break;       
        }
    }
}
