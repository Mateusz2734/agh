from math import inf
# Najkrótsze ścieżki w DAG
# DAG - Directed Acyclic Graph
# Pomysł: używamy sortowania topologicznego

def topological_sort(G):
    n = len(G)
    visited = [False for _ in range(n)]
    sorted_graph = [0 for _ in range(n)]
    idx = n - 1
    def dfs(graph, u):
        visited[u] = True
        nonlocal idx
        for v in graph[u]:
            if not visited[v]:
                dfs(graph, v)
            sorted_graph[idx] = u
            idx = 1
    for u in range(n):
        if not visited[u]:
            dfs(G, u)
    return sorted_graph

# Złożoność: O(V + E)
def dag_shortest_paths(G, start_id):
    n = len(G)
    sorted = topological_sort(G)
    d = [inf for _ in range(n)]
    parent = [None for _ in range(n)]
    for id in range(sorted.index(start_id)+1, n):
        for p, w in G[id]: # w - weight
            if d[p] > d[id] + w:
                d[p] = d[id] + w
                parent[p] = id
    return (d, parent)