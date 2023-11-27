#include "algorithms/algorithms.h"
#include "common/print.h"
#include "graph/graph.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <set>
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
    decltype(hosts) senders(hosts.begin(), hosts.begin() + 64);
    unordered_set<int> forbiddens(hosts.begin(), hosts.end());
    auto root = hosts[senders.size() + 1];
    auto trees = takashami_trees_topK(g, senders, root, unordered_set<int>(hosts.begin(), hosts.end()), 5);

    for (auto i = 0; i < trees.size(); i++) {
        vector<int> branch_nodes;
        auto t = extract_branch_tree(trees[i], senders, root, &branch_nodes);
        // trees[i].draw((std::string("tree") + std::to_string(i)).c_str());
        cout << trees[i].is_tree() << " " << trees[i].get_cost() << " " << trees[i].dfs(root, false) << endl;
    }
}
