from queue import PriorityQueue
from math import inf
from zad5testy import runtests

def relax(u, v, l, d, parent, queue: PriorityQueue):
    if d[v] > d[u] + l:
        d[v] = d[u] + l
        parent[v] = u
        queue.put((d[v], v))


def dijkstra(G, s):
    n = len(G)
    parent = [None for _ in range(n)]
    d = [inf for _ in range(n)]

    d[s] = 0
    queue = PriorityQueue()
    queue.put((d[s], s))

    while not queue.empty():
        du, u = queue.get()
        if d[u] == du:
            for v, l in G[u]:
                relax(u, v, l, d, parent, queue)
    return d

def spacetravel( n, E, S, a, b ):
    for i in S:
        for j in S:
            if i < j:
                E.append((i, j, 0))
    G = [[] for _ in range(n)]
    for u, v, l in E:
        G[u].append((v, l))
        G[v].append((u, l))
    d = dijkstra(G, a)
    return d[b] if d[b] != inf else None

# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( spacetravel, all_tests = False )