from kol2testy import runtests
from collections import deque
from math import inf


def bfs(G, s):
    n = len(G)
    visited = [False for _ in range(n)]
    parent = [None for _ in range(n)]
    Q = deque()

    visited[s] = True
    Q.append(s)

    while Q:
        u = Q.popleft()
        for v in G[u]:
            if not visited[v]:
                visited[v] = True
                parent[v] = u
                Q.append(v)
    return False not in visited


def get_edges(G):
    n = len(G)
    E = []
    for i in range(n):
        for elem in G[i]:
            if elem[0] > i:
                E.append((i, elem[0], elem[1]))
    return E


def build_graph(E, n, i):
    G = [[] for _ in range(n)]

    row_sum = 0
    for elem in E[i:n+i-1]:
        row_sum += elem[2]
        G[elem[0]].append(elem[1])
        G[elem[1]].append(elem[0])
    return G, row_sum


def beautree(G):
    n = len(G)

    E = get_edges(G)
    E.sort(key=lambda x: x[2])

    m = len(E)
    min_sum = inf
    for i in range(m-n):
        Gi, weight_sum = build_graph(E, n, i)
        if bfs(Gi, 0):
            if weight_sum < min_sum:
                min_sum = weight_sum
    return min_sum


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(beautree, all_tests=True)
