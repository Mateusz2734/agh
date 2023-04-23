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
            if not visited[v] and (u,v) != ignore and (v,u) != ignore:
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

def longer( G, s, t ):
    par, vis = bfs(G, s)
    if not vis[t]:
        return None
    
    # It will hold the path from t to s
    path = calc_path(par, s, t)
    length = len(path) - 1
    edges = [(path[i], path[i+1]) for i in range(len(path)-1)]

    for edge in edges:
        par, vis = bfs_ignore_edge(G, s, edge)
        if not vis[t]:
            return edge
        if len(calc_path(par, s, t)) > length:
            return edge


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests( longer, all_tests = True )

