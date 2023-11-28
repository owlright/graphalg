#include "algorithms.h"
#include "../common/print.h"
#include <algorithm>
#include <cmath>
#include <stack>
namespace graphalg::algorithms {

void floyd_warshall(double** distance, int n)
{
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Update the distance using vertex k as an intermediate point
                if (distance[i][k] != INFINITY && distance[k][j] != INFINITY) {
                    distance[i][j] = std::min(distance[i][j], distance[i][k] + distance[k][j]);
                }
            }
        }
    }
}

double dijistra(const Graph& g, int src, int dest, vector<int>* path)
{
    int n = g.get_max_vertice() + 1;
    priority_queue<diPair, std::deque<diPair>, CompareFirst<diPair>> pq;
    vector<double> dist(n, INFINITY);
    vector<int> prev(n, -1);
    pq.push(make_pair(0.0, src));
    dist[src] = 0;
    if (src == dest) {
        if (path)
            path->push_back(src);
        return 0;
    }
    bool found = false;
    unordered_set<int> visited { src };
    while (!pq.empty()) {
        auto u = pq.top().second;
        pq.pop();
        for (auto& [v, weight] : g.out_neighbors(u)) {
            // If there is shorted path to v through u.
            if (visited.find(v) == visited.end() && dist[v] > dist[u] + weight) {
                // Updating distance of v
                visited.insert(v);
                dist[v] = dist[u] + weight;
                prev[v] = u;
                pq.push(make_pair(dist[v], v));
            }
            if (v == dest) {
                found = true;
                break;
            }
        }
        if (found) {
            break;
        }
    }

    if (found) {
        if (path) {
            auto u = dest;
            while (u != -1) {
                path->insert(path->begin(), u);
                u = prev[u];
            }
        }
        return dist[dest];
    }
    return INFINITY; // * unreachable
}

vector<double> yenksp(Graph& g, int src, int dest, int K, vector<Path>& A)
{
    vector<double> distances;
    Path p;
    vector<CostPath> B;
    distances.push_back(dijistra(g, src, dest, &p));
    A.push_back(p);
    for (int k = 1; k < K; k++) {
        auto& prevPath = A.at(k - 1);
        for (int i = 0; i < prevPath.size() - 1; i++) {
            int spurNode = prevPath[i];
            vector<int> rootPath(prevPath.begin(), prevPath.begin() + i + 1);
            auto rootPathCost = dijistra(g, rootPath.front(), rootPath.back());
            vector<double> weights_to_delete;
            vector<Edge> edges_to_delete;
            for (auto& p : A) {
                vector<int> pslice(p.begin(), p.begin() + i + 1);
                if (rootPath == pslice) {
                    auto u = p.at(i);
                    auto v = p.at(i + 1);
                    if (g.has_edge(u, v)) {
                        edges_to_delete.push_back({ u, v });
                        weights_to_delete.push_back(g.weight(u, v));
                        g.remove_edge(u, v);
                    }
                }
            }
            // * remove rootPathNode from Graph except spurNode
            for (auto& rootPathNode : rootPath) {
                if (rootPathNode == spurNode) {
                    continue;
                }
                for (auto& [v, w] : g.out_neighbors(rootPathNode)) {
                    edges_to_delete.push_back({ rootPathNode, v });
                    weights_to_delete.push_back(w);
                }
                for (auto& [v, w] : g.in_neighbors(rootPathNode)) {
                    edges_to_delete.push_back({ v, rootPathNode });
                    weights_to_delete.push_back(w);
                }
                g.remove_node(rootPathNode);
            }

            vector<int> spurPath;
            auto spurPathCost = dijistra(g, spurNode, dest, &spurPath);
            if (spurPathCost != INFINITY) { // ! there may be no path from spurNode to dest
                spurPathCost += rootPathCost;
                ASSERT(spurPath.size() >= 2);
                vector<int> totalPath(rootPath.begin(), rootPath.end());
                totalPath.insert(totalPath.end(), spurPath.begin() + 1, spurPath.end());
                bool found = false;
                for (auto& [c, x] : B) {
                    if (x == totalPath) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    B.push_back({ spurPathCost, totalPath });
                }
            } else {
                // do nothing
                ASSERT(spurPath.empty());
            }
            for (int index = 0; index < weights_to_delete.size(); index++) {
                g.add_edge(edges_to_delete[index].first, edges_to_delete[index].second, weights_to_delete[index]);
            }
        }
        if (B.empty())
            break;
        std::sort(B.begin(), B.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        distances.push_back(B[0].first);
        A.push_back(B[0].second);
        B.erase(B.begin());
    }
    return distances;
}

Graph takashami_tree(const Graph& g, vector<int> sources, int root)
{
    Graph tree;
    tree.add_node(root);
    std::unordered_set<int> visited;
    for (auto i = 0; i < sources.size(); i++) {
        double min_dist = INFINITY;
        int node_in_tree = -1;
        int min_src = -1;

        for (auto& s : sources) {
            if (visited.find(s) == visited.end()) {
                for (auto& n : tree.get_nodes()) {
                    auto curr_dist = g.distance(s, n);
                    if (visited.find(n) == visited.end() && min_dist > curr_dist) {
                        min_dist = curr_dist;
                        node_in_tree = n;
                        min_src = s;
                    }
                }
            }
        }
        ASSERT(min_src != -1);
        visited.insert(min_src);
        std::vector<int> path;
        dijistra(g, min_src, node_in_tree, &path);

        if (path.size() < 2) {
            throw cRuntimeError("graph is not connected");
        }
        for (int i = 0; i < path.size() - 1; i++) {
            tree.add_edge(path[i], path[i + 1], g.weight(path[i], path[i + 1]));
        }
    }

    if (!tree.is_tree()) {
        tree.draw("wrong.png");
    }
    ASSERT(tree.is_tree());

    return tree;
}

vector<Graph> takashami_trees(const Graph& g, vector<int> sources, int root, const unordered_set<int>& forbiddens,
    vector<map<int, vector<int>>>* equal_branch_nodes)
{
    vector<Graph> trees {};

    std::unordered_set<Graph, Graph::Hash> visitedBranchTrees;
    std::queue<Graph> waited;
    waited.push(takashami_tree(g, sources, root));

    while (!waited.empty()) {
        auto t = waited.front();
        waited.pop();
        vector<int> branch_nodes;
        auto branch_tree = extract_branch_tree(t, sources, root, &branch_nodes);
        if (visitedBranchTrees.find(branch_tree) == visitedBranchTrees.end()) {
            visitedBranchTrees.insert(branch_tree);
            trees.push_back(t);
            if (equal_branch_nodes) {
                equal_branch_nodes->push_back(map<int, vector<int>>());
            }
            unordered_set<int> forbiddenmore(forbiddens.begin(), forbiddens.end());
            for (auto& b : branch_nodes) {
                forbiddenmore.insert(b);
            }
            for (auto& b : branch_nodes) {
                vector<int> equals = find_equal_nodes(g, branch_tree, b, forbiddenmore);
                if (equal_branch_nodes) {
                    equal_branch_nodes->back()[b] = equals;
                }
                auto gcopy = g;
                gcopy.remove_node(b);
                for (auto n : equals) {
                    gcopy.remove_node(n);
                }
                try {
                    std::vector<int> newBranchNodes;
                    auto newt = takashami_tree(gcopy, sources, root);
                    auto newBranchTree = extract_branch_tree(newt, sources, root, &newBranchNodes);
                    if (visitedBranchTrees.find(newBranchTree) == visitedBranchTrees.end()) {
                        waited.push(newt);
                    }
                } catch (cRuntimeError& e) {
                    continue;
                }
            }
        }
    }
    return trees;
}

vector<Graph> takashami_tree_K(const Graph& g, vector<int> sources, int root, int K)
{
    using ::operator<<;
    vector<Graph> trees { takashami_tree(g, sources, root) };
    vector<int> branchNodes;

    unordered_set<int> hosts;
    for (auto& n : g.get_nodes()) {
        if (g.outdegree(n) == 1) {
            hosts.insert(n);
        }
    }
    auto t = trees.back();
    auto branchTree = extract_branch_tree(t, sources, root, &branchNodes);
    std::unordered_set<Graph, Graph::Hash> visitedBranchTrees;
    visitedBranchTrees.insert(branchTree);
    unordered_set<int> visitedBranchNodes;
    std::queue<int> waited;
    for (auto& n : branchNodes) {
        waited.push(n);
    }
    while (!waited.empty()) {
        auto b = waited.front();
        waited.pop();
        if (visitedBranchNodes.find(b) == visitedBranchNodes.end()) {
            visitedBranchNodes.insert(b);
            vector<int> equals = find_equal_nodes(g, branchTree, b, hosts);
            auto gcopy = g;
            gcopy.remove_node(b);
            for (auto n : equals) {
                gcopy.remove_node(n);
            }
            try {
                std::vector<int> newBranchNodes;
                auto newt = takashami_tree(gcopy, sources, root);
                auto newBranchTree = extract_branch_tree(newt, sources, root, &newBranchNodes);
                if (visitedBranchTrees.find(newBranchTree) == visitedBranchTrees.end()) {
                    trees.push_back(newt);
                    visitedBranchTrees.insert(newBranchTree);
                    branchNodes.insert(branchNodes.end(), newBranchNodes.begin(), newBranchNodes.end());
                }
            } catch (cRuntimeError& e) {
                continue;
            }
        }
    }
    return trees;
}

Graph extract_branch_tree(const Graph& tree, const vector<int>& sources, int root, vector<int>* branch_nodes)
{
    // ! make sure tree is actually a directed tree
    Graph t;
    unordered_set<int> visited;
    for (auto& s : sources) {
        auto node = s;
        int edge_start = s;
        while (node != root) {
            if (visited.find(node) != visited.end()) {
                t.add_edge(edge_start, node, tree.distance(edge_start, node));
                break;
            } else {
                visited.insert(node);
            }
            if (tree.indegree(node) > 1) {
                if (branch_nodes) {
                    branch_nodes->push_back(node);
                }
                t.add_edge(edge_start, node, tree.distance(edge_start, node));
                edge_start = node;
            }
            node = tree.out_neighbors(node).at(0).first;
        }
        if (node == root) {
            t.add_edge(edge_start, node, tree.distance(edge_start, node));
        }
    }
    return t;
}

vector<int> find_equal_nodes(
    const Graph& g, const Graph& tree, int node, const std::unordered_set<int>& forbiddens, double threshold)
{
    vector<int> equal_nodes;
    if (forbiddens.find(node) != forbiddens.end())
        return equal_nodes;
    std::vector<int> children;
    auto dist = g.get_dist();
    int parent = tree.out_neighbors(node).at(0).first;
    double orig_cost = tree.out_neighbors(node).at(0).second;
    for (auto& [v, w] : tree.in_neighbors(node)) {
        orig_cost += w;
        children.push_back(v);
    }

    for (auto& i : g.get_nodes()) {
        ASSERT(dist);
        if (forbiddens.find(i) == forbiddens.end() && i != node) {
            double temp_cost = dist[i][parent];
            for (auto& c : children) {
                temp_cost += dist[c][i];
            }
            if (std::abs(temp_cost - orig_cost) <= threshold) {
                equal_nodes.push_back(i);
            }
        }
    }
    return equal_nodes;
}

Graph edmonds(Graph& g)
{
    Graph dmst;
    auto n = g.get_vertices_number();
    vector<int> in(n, -1);
    vector<double> const_cost(n, 0);
    vector<int> prev(n, -1);
    vector<int> parent(n, -1);
    vector<vector<int>> children(n, vector<int>());
    return dmst;
}

}
