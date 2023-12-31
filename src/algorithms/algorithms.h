#pragma once
#include "../common/utils.h"
#include "../graph/graph.h"
#include <iostream>
#include <queue>
#include <unordered_set>
#include <utility>
#include <vector>
namespace graphalg {

namespace algorithms {
    using std::make_pair;
    using std::pair;
    using std::priority_queue;
    using std::unordered_set;
    using std::vector;
    using Path = vector<int>;
    using CostPath = pair<double, Path>;

    template <typename K, typename V>
    std::ostream& operator<<(std::ostream& os,
        const std::priority_queue<std::pair<K, V>, std::deque<std::pair<K, V>>, CompareFirst<std::pair<K, V>>>& pq)
    {
        std::priority_queue<std::pair<K, V>, std::deque<std::pair<K, V>>, CompareFirst<std::pair<K, V>>> tmpQueue = pq;

        while (!tmpQueue.empty()) {
            os << "(" << tmpQueue.top().first << ", " << tmpQueue.top().second << ") ";
            tmpQueue.pop();
        }

        return os;
    }

    void floyd_warshall(double** distance, int n);
    double dijistra(const Graph& g, int src, int dest, vector<int>* path = nullptr);
    Graph takashami_tree(const Graph& g, vector<int> sources, int root);
    vector<Graph> takashami_trees(const Graph& g, vector<int> sources, int root, const unordered_set<int>& forbiddens = unordered_set<int>(), vector<map<int, vector<int>>>* equal_branch_nodes = nullptr);
    Graph extract_branch_tree(
        const Graph& tree, const vector<int>& sources, int root, vector<int>* branch_nodes = nullptr);
    vector<double> yenksp(Graph& g, int src, int dest, int K, vector<Path>& A);
    vector<int> find_equal_nodes(const Graph& g, const Graph& tree, int node,
        const std::unordered_set<int>& forbiddens = std::unordered_set<int>(), double threshold = 0.0);

}

}