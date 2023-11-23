from net import *
import random

fat = fattree(14, singleHost=False)
hosts = [h for h, att in fat.nodes(data=True) if att['type']=='host']
random.seed(1235)
random.shuffle(hosts)
terminals = hosts[:16]
mst = nx.approximation.steinertree.steiner_tree(fat, terminals)
fig, ax = plt.subplots()

highlight(fat, mst, ax=ax)
plt.show()