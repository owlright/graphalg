#pragma once
#include "../common/utils.h"
#include <map>
#include <queue>
#include <unordered_set>

namespace graphalg {
using std::map;
using std::pair;
using Node = int;
using Edge = pair<int, int>;
using EdgeWeight = pair<int, double>;
class Graph {
public:
    void read_dot(const char* filename);
    vector<Node> dfs(Node root, bool directionOut = true) const;
    vector<Node> bfs(Node root, bool directionOut = true) const;
    bool is_tree() const;
    bool is_connected() const;

    bool operator==(const Graph& other) const
    { // ! weights are not considered here
        // Compare the adjacency lists
        if (adjout.size() != other.adjout.size())
            return false;
        for (const auto& [src, vw1] : adjout) {
            auto it = other.adjout.find(src);
            if (it == other.adjout.end()) {
                return false;
            }
            auto& vw2 = it->second;
            if (vw1.size() != vw2.size()) {
                return false;
            }
            std::unordered_set<int> s1;
            std::unordered_set<int> s2;
            for (auto i = 0; i < vw1.size(); i++) {
                s1.insert(vw1[i].first);
                s2.insert(vw2[i].first);
            }
            if (s1 != s2) {
                return false;
            }
        }
        return true;
    }

    // 实现哈希函数
    struct Hash {
        size_t operator()(const Graph& graph) const
        {
            size_t hash = 0;
            for (const auto& node : graph.nodes) {
                // 基于键和值的哈希值计算
                hash ^= std::hash<int>()(node);
            }
            return hash;
        }
    };

public:
    void add_edge(Node src, Node dest, double weight = 1.0, bool bidirectional = false);
    void set_weight(Node src, Node dest, double weight = 1.0);
    void remove_edge(Node src, Node dest);
    void add_node(Node n);
    void remove_node(Node n);
    bool has_node(Node n) const;
    bool has_edge(const Node& src, const Node& dest) const;
    bool has_edge(const Edge&) const;

    void update_dist();
    double distance(Node src, Node dest) const;
    double weight(Node src, Node dst) const;
    int get_vertices_number() const { return adjout.size(); }
    int get_max_vertice() const { return max_vertice; }

public:
    const vector<Node>& get_nodes() const { return nodes; }

    const vector<EdgeWeight>& out_neighbors(Node src) const
    {
        if (adjout.find(src) != adjout.end()) {
            return adjout.at(src);
        } else {
            return emptyEdge;
        }
    }
    const vector<EdgeWeight>& in_neighbors(Node src) const
    {
        if (adjin.find(src) != adjin.end()) {
            return adjin.at(src);
        }
        return emptyEdge;
    }
    int indegree(Node v) const { return adjin.at(v).size(); }
    int outdegree(Node v) const { return adjout.at(v).size(); }
    double get_cost() const;

public:
    void draw(const char* filename, const char* engine = "neato");

private:
    double** dist { nullptr };
    vector<Node> nodes;
    Node max_vertice { -1 };
    map<Node, vector<EdgeWeight>> adjout;
    map<Node, vector<EdgeWeight>> adjin;
    vector<EdgeWeight> emptyEdge;

private:
    void init_weight_matrix();
    void reset_dist();

public:
    explicit Graph();
    ~Graph();
    // copy constructor
    Graph(const Graph& other);
    // move constructor
    Graph(Graph&& other);
    // copy assignment operator
    // a = b will call this;
    Graph& operator=(const Graph& other);
    // move assignment operator
    // a = std::move(b)
    // auto a = getNetworkCopy() will call this
    Graph& operator=(Graph&& other);
};

}