#include "algorithms/algorithms.h"
#include "graph/graph.h"
#include "common/print.h"
#include <algorithm>
#include <iostream>
#include <random>
#include <unordered_set>
using namespace graphalg;
using namespace graphalg::algorithms;
using std::cout;
using std::endl;
using std::unordered_set;
int main()
{
    Graph g;
    g.read_dot("network.dot");
    g.update_dist();

    vector<int> hosts;
    for (auto& n : g.get_nodes()) {
        if (g.outdegree(n) == 1) {
            hosts.push_back(n);
        }
    }
    auto now = 2343;
    std::shuffle(hosts.begin(), hosts.end(), std::default_random_engine(now));
    decltype(hosts) senders(hosts.begin(), hosts.begin() + 20);
    unordered_set<int> forbiddens(hosts.begin(), hosts.end());
    auto root = hosts[21];
    auto trees = takashami_tree_K(g, senders, root, 20);

    for (auto i = 0; i < trees.size(); i++) {
        vector<int> branch_nodes;
        auto t = extract_branch_tree(trees[i], senders, root, &branch_nodes);
        // trees[i].draw((std::string("tree") + std::to_string(i)).c_str());
        cout << cost(trees[i]) << endl;
    }
}
