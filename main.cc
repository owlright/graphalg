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
    g.read_dot("K8.dot");
    g.update_dist();

    vector<int> hosts;
    for (auto& n : g.get_nodes()) {
        if (g.outdegree(n) == 1) {
            hosts.push_back(n);
        }
    }
    auto now = 234158;
    std::shuffle(hosts.begin(), hosts.end(), std::default_random_engine(now));
    decltype(hosts) senders(hosts.begin(), hosts.begin() + 60);
    unordered_set<int> forbiddens(hosts.begin(), hosts.end());
    auto root = hosts[senders.size() + 1];
    vector<map<int, vector<int>>> equal_branch_nodes;

    auto tree = takashami_tree(g, senders, root);
    double cost = 0;
    for (auto s : senders)
        cost += g.distance(s, root);
    cout << "no compressed tree cost " << cost << endl;
    // ASSERT(trees.size() == equal_branch_nodes.size());
    // for (auto i = 0; i < trees.size(); i++) {
    //     vector<int> branch_nodes;
    //     auto t = extract_branch_tree(trees[i], senders, root, &branch_nodes);
    //     cout << "tree " << i << " " << trees[i].get_cost() << endl;
    //     trees[i].draw((std::string("tree") + std::to_string(i) + ".png").c_str());
    //     // t.draw((std::string("branchtree") + std::to_string(i) + ".png").c_str());
    //     // vector<int> path;
    //     // dijistra(t, senders[1], root, &path);

    //     cout << equal_branch_nodes[i] << endl;
    // }
}
