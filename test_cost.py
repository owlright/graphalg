# from net.util.plotters import plot, highlight
import networkx as nx
import matplotlib.pyplot as plt
import random
from pprint import pprint


def reduced_cost(g: nx.Graph, agg_nodes: [int], S: [int], d: int) -> int:
    spl = {s: nx.dijkstra_path_length(g, s, d) for s in S}
    cost0 = sum([v for k, v in spl.items()])
    t = nx.approximation.steiner_tree(g, agg_nodes + [d])
    cost1 = sum(v for _, v in nx.get_edge_attributes(t, 'weight'))
    for s in S:
        lengths, _= nx.multi_source_dijkstra(g, agg_nodes + [d], s)
        cost1 += min([v for k, v in lengths.items()])
    return cost1 - cost0


random.seed(233)
g = nx.Graph()
g.add_edges_from(
    [
        (1, 9),
        (1, 6),
        (2, 6),
        (3, 6),
        (3, 7),
        (4, 7),
        (5, 7),
        (5, 8),
        (5, 10),
        (6, 8),
        (7, 8),
        (6, 9),
        (8, 9),
        (8, 10),
        (9, 10),
    ]
)
weights = {e: random.randint(1, 5) for e in g.edges()}
pprint(weights)
# pos = nx.kamada_kawai_layout(g)
# nx.set_node_attributes(g, pos, "pos")
nx.set_edge_attributes(g, weights, "weight")
S = [1, 2, 3, 4, 5]
V = [6, 7, 8, 9]
r = 10
total_cost = sum([nx.dijkstra_path_length(g, s, r) for s in S])
for a in V:
    print(reduced_cost(g, [a], S, r))
