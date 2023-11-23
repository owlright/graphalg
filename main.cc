#include "graph/graph.h"

int main()
{
    Graph g;
    enum {
        C,
        D,
        E,
        F,
        G,
        H,
    };
    g.add_edge(C, D, 3);
    g.add_edge(D, F, 4);
    g.add_edge(C, E, 2);
    g.add_edge(E, D, 1);
    g.add_edge(E, F, 2);
    g.add_edge(E, G, 3);
    g.add_edge(F, G, 2);
    g.add_edge(G, H, 2);
    g.add_edge(F, H, 1);
    g.draw("example");
}
