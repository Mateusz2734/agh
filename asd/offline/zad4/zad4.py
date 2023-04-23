# Mateusz Wala
# Zamysł algorytmu: za pomocą BFS znaujdujemy najkrótszą (w sensie ilości krawędzi) ścieżkę pomiędzy wierzchołkami s i t.
# Następnie dla każdej krawędzi na tej ścieżce sprawdzamy czy istnieje ścieżka pomiędzy s i t, która nie przechodzi
# przez tę krawędź oraz czy jest ona dłuższa od najkrótszej znalezionej ścieżki.
# Zwracamy krawędź, która stworzy najdłuższą ścieżkę pomiędzy s i t.
from collections import deque
from zad4testy import runtests


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
    return (parent, visited)


def bfs_ignore_edge(G, s, ignore):
    n = len(G)
    visited = [False for _ in range(n)]
    parent = [None for _ in range(n)]
    Q = deque()

    visited[s] = True
    Q.append(s)

    while Q:
        u = Q.popleft()
        for v in G[u]:
            if not visited[v] and (u, v) != ignore and (v, u) != ignore:
                visited[v] = True
                parent[v] = u
                Q.append(v)
    return (parent, visited)


def calc_path(parent, s, t):
    path = []
    while t != parent[s]:
        path.append(t)
        t = parent[t]
    return path


def longer(G, s, t):
    par, vis = bfs(G, s)
    if not vis[t]:
        return None

    path = calc_path(par, s, t)
    max_length = len(path) - 1
    to_remove = None
    edges = [(path[i], path[i + 1]) for i in range(len(path) - 1)]

    for edge in edges:
        par, vis = bfs_ignore_edge(G, s, edge)
        if not vis[t]:
            return edge
        curr_length = len(calc_path(par, s, t)) - 1
        if curr_length > max_length:
            max_length = curr_length
            to_remove = edge
    return to_remove


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(longer, all_tests=True)
