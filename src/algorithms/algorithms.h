#pragma once
#include "../common/utils.h"
#include "../graph/graph.h"
#include <queue>

namespace graphalg {

namespace algorithms {
    using std::make_pair;
    using std::pair;
    using std::priority_queue;
    using std::unordered_set;
    using diPair = pair<double, int>;
    using Path = vector<int>;
    using CostPath = pair<double, Path>;
    using std::vector;

    void floyd_warshall(double** distance, int n);
    double dijistra(const Graph& g, int src, int dest, vector<int>* path = nullptr);
    Graph takashami_tree(const Graph& g, vector<int> sources, int root);
    Graph extract_branch_tree(
        const Graph& tree, const vector<int>& sources, int root, vector<int>* branch_nodes = nullptr);
    vector<double> yenksp(Graph& g, int src, int dest, int K, vector<Path>& A);
    vector<int> find_equal_nodes(const Graph& g, const Graph& tree, int node,
        const std::unordered_set<int>& forbiddens = std::unordered_set<int>(), double threshold = 0.0);
    Graph edmonds(Graph& g);

}

}