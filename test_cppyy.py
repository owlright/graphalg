import cppyy

cppyy.include("src/graph/graph.h")
cppyy.load_library("libgraph.so")
from cppyy.gbl import Graph

g = Graph()
g.add_edge(1,2,3)
g.add_edge(1,3, 4)
g.draw("hello")