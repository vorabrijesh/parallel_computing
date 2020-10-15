import numpy as np
import matplotlib.pyplot as plt
import scipy.linalg as la
import networkx as nx

#G = nx.complete_graph(3)
#G = nx.karate_club_graph()
#G = nx.random_powerlaw_tree(4)
G = nx.random_regular_graph(2,3)
nx.draw(G, with_labels=True)
plt.show()

A = nx.adjacency_matrix(G).todense()
f = open("file.txt","w")
s = str(A)
f.write(s);
f.close();
print(A)
print(A.shape)
print(type(A))
rows, cols = A.shape
print(rows)

s = 0
d = 3 
Ak = A
length = 1

while Ak[s,d] == 0 and length != rows:
    Ak = Ak @ A
    length = length + 1
    print("AK:")
    print(Ak)

print(nx.single_source_shortest_path(G, 0))

if length == rows:
    print("No path between the source and destination")
else:
    print(length)

