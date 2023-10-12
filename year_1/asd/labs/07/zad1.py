from copy import deepcopy
from math import inf
# Algorytm Forda-Fulkersona z macierzą sąsiedztwa

def find_path(G, s, t):
    # BFS lub DFS
    pass

def min_weight(G, path):
    w = G[path[0]][path[1]]
    for i in range(1, len(path) - 1):
        w = min(w, G[path[i]][path[i + 1]])
    return w

def update_weights(G, path):
    w = min_weight(G, path)
    for i in range(len(path) - 1):
        G[path[i]][path[i + 1]] -= w
        G[path[i + 1]][path[i]] += w

def ford_fulkerson(M, s, t):
    n = len(M)
    G = deepcopy(M)
    
    flow = 0
    my_path = find_path(G, s, t)
    while my_path:
        flow += min_weight(G, my_path)
        update_weights(G, my_path)

        my_path = find_path(G, s, t)
    return flow
