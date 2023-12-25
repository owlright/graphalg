import networkx as nx
import matplotlib.pyplot as plt
from net import *
from heapq import heapify, heappush, heappop
from pprint import pprint


class weightedUF:
    _id = []
    _sz = []
    _count = 0

    def __init__(self, N):
        self._id = list(range(0, N + 1))
        self._count = N
        self._sz = [1] * (N + 1)
        self._sz[0] = 0

    # Quick-union
    def find(self, p):
        while self._id[p] != p:
            p = self._id[p]
        return p

    def union(self, p, q):
        self._rootP = self.find(p)
        self._rootQ = self.find(q)

        if self._rootP == self._rootQ:
            return None

        if self._sz[self._rootP] < self._sz[self._rootQ]:
            self._id[self._rootP] = self._rootQ
            self._sz[self._rootQ] += self._sz[self._rootP]
        else:
            self._id[self._rootQ] = self._rootP
            self._sz[self._rootP] += self._sz[self._rootQ]

        self._count = self._count - 1

    def connected(self, p, q):
        return self.find(p) == self.find(q)

    def count(self):
        return self._count


lecture = nx.DiGraph()
lecture.add_edges_from(
    [
        (1, 2),
        (2, 1),
        (2, 4),
        (3, 2),
        (3, 5),
        (4, 3),
        (5, 4),
        (5, 9),
        # (6, 2),
        (6, 8),
        (7, 6),
        (8, 7),
        # (9, 7),
    ]
)

grid = nx.DiGraph()
grid.add_edge(1, 3, weight=5)
grid.add_edge(1, 2, weight=11)
grid.add_edge(2, 3, weight=6)
grid.add_edge(3, 5, weight=3)
grid.add_edge(3, 6, weight=13)
grid.add_edge(4, 3, weight=2)
grid.add_edge(4, 2, weight=10)
grid.add_edge(5, 6, weight=9)
grid.add_edge(5, 7, weight=12)
grid.add_edge(6, 4, weight=7)
grid.add_edge(6, 8, weight=8)
grid.add_edge(7, 8, weight=1)
grid.add_edge(8, 5, weight=4)
for i in range(1, 9):
    grid.add_node(i, pos=(i / 2, i % 2))


def edmond_tarjan(g: nx.DiGraph, root: int) -> nx.DiGraph:
    # initialize
    _in = {n: tuple() for n in g}
    cost = {n: 0 for n in g}  # weight entering u
    prev = {n: -1 for n in g}  # maybe super vertex
    parent = {n: -1 for n in g}
    children = {n: [] for n in g}
    P = {n: [(g.get_edge_data(u, v, default=1).get("weight", 1), u, v) for u, v in g.in_edges(n)] for n in g}
    for k, v in P.items():
        heapify(v)

    def new_vertex(vertex: int) -> int:
        _in[vertex] = ()
        cost[vertex] = 0  # weight entering u
        prev[vertex] = -1  # (super-)vertex preced me
        parent[vertex] = -1
        children[vertex] = []
        P[vertex] = []
        return vertex

    def find(u: int) -> int:
        while parent[u] != -1:
            u = parent[u]
        return u

    def weight(u, v) -> int:
        w = g.get_edge_data(u, v, default=1).get("weight", 1)
        while parent[v] != -1:
            w += cost[u]
            v = parent[v]
        return w

    def meld(P: dict, c, a):
        for x in P[a]:
            heappush(P[c], x)
        P[a].clear()

    def contract():
        a = 3  # start with an arbitrary node
        curr_max_vertex = g.number_of_nodes() + 1
        count = 1
        while len(P[a]) > 0:
            _, u, v = heappop(P[a])
            print(f"{count} ({a}) ({u}, {v})")
            # print(f"select ({u}, {v})")
            top_parent = find(u)
            # print(f"top parent[{u}] is {branch}")
            if a != top_parent:  # u, v is not a self-loop
                _in[a] = u, v
                # print(f"in[{a}] = ({u}, {v})")
                prev[a] = top_parent
                if len(_in[u]) == 0:  # Path extended
                    # print(f"extend {a} to {branch}")
                    a = top_parent
                else:
                    # ------------------------- debug: show found circle ------------------------- #
                    circle = [a]
                    _tmp = a
                    while _in[_tmp][0] != a:
                        circle.append(_in[_tmp][0])
                        _tmp = _in[_tmp][0]
                    print(circle)
                    # ---------------------------------------------------------------------------- #
                    c = new_vertex(curr_max_vertex)
                    curr_max_vertex += 1
                    while parent[a] == -1:
                        parent[a] = c
                        cost[a] = -weight(*_in[a])  # adjust node c's in_edges' cost in case to calc twice?
                        children[c].append(a)
                        meld(P, c, a)
                        a = prev[a]
                    a = c  # replace the circle with this contracted_node

    # why need this? we can store the branch nodes when creating them
    def dismantle(u: int, R: set):
        while parent[u] != -1:
            u = parent[u]
            for v in children[u]:
                parent[v] = -1
                if len(children[v]) != 0:
                    R.add(v)

    def expand(r: int) -> nx.DiGraph:
        R = set(parent)
        # dismantle(r, R)
        t = nx.DiGraph()
        # while R:
        #     c = R.pop()
        #     u, v = _in[c]
        #     _in[v] = u, v
        #     t.add_edge(u, v)
        #     dismantle(v, R)
        return t

    contract()
    print(_in)

    return expand(root)


def tarjan_branch(g: nx.DiGraph):
    # initialize
    S_set = weightedUF(g.number_of_nodes())  # store circles if exist
    W_set = weightedUF(g.number_of_nodes())
    enter = {i: (0, 0) for i in g}
    _min = {i: i for i in g}  # maybe super vertex
    roots = set(g)
    cost = nx.get_edge_attributes(g, "weight")
    Q = {n: [(cost[e], e[0], e[1]) for e in g.in_edges(n)] for n in g}
    for _, v in Q.items():
        heapify(v)

    def SFIND(i):
        return S_set.find(i)

    def WFIND(i):
        return W_set.find(i)

    def WUNION(i, j):
        return W_set.union(i, j)

    def SUNION(i, j):
        return S_set.union(i, j)

    def QUNION(i, j):
        for c, u, v in Q[j]:
            heappush(Q[i], (c, u, v))

    def ADD(val, k):
        _tmp = []
        for c, u, v in Q[k]:
            print(f"reduce {u,v} {c} to {c+val}")
            _tmp.append((c + val, u, v))
        Q[k] = _tmp

    H = set()
    rset = set()
    while roots:
        k = roots.pop()
        if not Q[k]:
            continue
        _, i, j = heappop(Q[k])
        print(f"{i, j}")
        if cost[i, j] <= 0:
            rset.add(k)
        elif SFIND(i) == k:
            print(f"{i}is in strongly connected component {k}")
            roots.add(k)
        else:
            H.add((i, j))
            if WFIND(i) != WFIND(j):
                WUNION(WFIND(i), WFIND(j))
                enter[k] = (i, j)
            else:
                print(f"{i} {j} in the same weakly connected set {WFIND(i)}")
                print(f"cost{i,j}={cost[i, j]}")
                val = 1e10
                vertex = None
                x, y = i, j
                # find edge with min cost
                while (x, y) != (0, 0):
                    if cost[x, y] < val:
                        val = cost[x, y]
                        vertex = SFIND(y)
                    x, y = enter[SFIND(x)]
                print(f"minval={val}")
                ADD(val - cost[i, j], k)
                _min[k] = _min[vertex]
                x, y = enter[SFIND(i)]
                print(f"strongly connected set {SFIND(i)}'s in_edge {x, y} ")
                while (x, y) != (0, 0):
                    ADD(val - cost[x, y], SFIND(y))
                    QUNION(k, SFIND(y))
                    print(f"{k}<-{SFIND(y)}")
                    SUNION(k, SFIND(y))
                    x, y = enter[SFIND(x)]
                roots.add(k)
                print()
    print(enter)
    print(_min)


# t = edmond_tarjan(grid, 4)
tarjan_branch(grid)
# da = weightedUF(lecture.number_of_nodes())
# for u, v in lecture.edges():
#     print(u, v)
#     da.union(u, v)
# print(da._id)
# print(da._sz)
