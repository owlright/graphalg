#pragma once
#include "../graph/graph.h"

namespace algorithms {
using Path = vector<int>;
void floyd_warshall(Mat<double>& distance);
double dijistra(const Graph& g, int src, int dest, vector<int>* path = nullptr);
Graph takashami_tree(const Graph& g, vector<int> sources, int root);
Graph extract_branch_tree(const Graph& tree, const vector<int>& sources, int root, vector<int>* branch_nodes = nullptr);
vector<double> yenksp(Graph& g, int src, int dest, int K, vector<Path>& A);
vector<int> find_equal_nodes(const Graph& g, const Graph& tree, int node);
Graph edmonds(Graph&g);
}