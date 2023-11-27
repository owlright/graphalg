#pragma once
#include "../common/utils.h"
#include <map>
#include <queue>
#include <unordered_set>

namespace graphalg {
using std::map;
using std::pair;
using Edge = pair<int, int>;
using EdgeWeight = pair<int, double>;
class Graph {
public:
    void read_dot(const char* filename);
    vector<int> dfs(int root, bool directionOut = true) const;
    vector<int> bfs(int root, bool directionOut = true) const;
    bool is_tree() const;

public:
    void add_edge(int src, int dest, double weight = 1.0, bool bidirectional = false);
    void set_weight(int src, int dest, double weight = 1.0);
    void remove_edge(int src, int dest);
    void add_node(int n);
    void remove_node(int n);
    bool has_node(int n) const;
    bool has_edge(const int& src, const int& dest) const;
    bool has_edge(const Edge&) const;

    void update_dist();
    double distance(int src, int dest) const;
    double weight(int src, int dst) const;
    int get_vertices_number() const { return adjout.size(); }
    int get_max_vertice() const { return max_vertice; }

public:
    const vector<int>& get_nodes() const { return nodes; }
    double** get_dist() const;
    const vector<EdgeWeight>& out_neighbors(int src) const
    {
        if (adjout.find(src) != adjout.end()) {
            return adjout.at(src);
        } else {
            return emptyEdge;
        }
    }
    const vector<EdgeWeight>& in_neighbors(int src) const
    {
        if (adjin.find(src) != adjin.end()) {
            return adjin.at(src);
        }
        return emptyEdge;
    }
    int indegree(int v) const { return adjin.at(v).size(); }
    int outdegree(int v) const { return adjout.at(v).size(); }
    double get_cost() const;

public:
    void draw(const char* filename, const char* engine = "neato");

private:
    double** dist { nullptr };
    vector<int> nodes;
    int max_vertice { -1 };
    map<int, vector<EdgeWeight>> adjout;
    map<int, vector<EdgeWeight>> adjin;
    vector<EdgeWeight> emptyEdge;

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