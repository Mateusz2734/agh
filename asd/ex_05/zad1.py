# Znaleźć ścieżkę Hamiltona w DAG, gdzie
# DAG - Directed Acyclic Graph

# Pomysł: sortowanie topologiczne, ale wybieramy wierzchołek, który ma stopień wejściowy zero
# Sprawdzamy, czy da się przejść od pierwszego do ostatniego

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

def hamiltonian_path(G):
    t = topological_sort(G)
    for i in range(len(t) - 1):
        u = t[i]
        v = t[i + 1]
        if v not in G(u):
            return False
    return True
