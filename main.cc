#include "algorithms/algorithms.h"
#include "graph/graph.h"
#include <cstdlib>
#include <iostream>
int main()
{
    Graph g;
    g.read_dot("network.dot");
    vector<algorithms::Path> A;
    algorithms::yenksp(g, 16, 128, 4, A);
    for (auto p : A) {
        std::cout << p << std::endl;
    }
}
