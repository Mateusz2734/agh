# Czy graf ma dobry początek?
# Wierzchołek v jest dobrym początkiem, jeśli do każdego 
# wierzchołka można trafić ścieżką skierowaną z v

# Pomysł A: "Tworzymy" silnie spójne składowe

# Pomysł B: Odpalamy dfs (w pętli, żeby nie pominąć) razem z czasami przetworzenia (post-order) i 
# znajdujemy wierzchołek z najwyższym czasem
# Z tego wierzchołka odpalamy DFS lub BFS i sprawdzamy, czy można wszędzie dotrzeć

def dfs_visit(G, u, time, time_visited, visited):
    visited[u] = True

    for v in G[u]:
        if not visited[v]:
            time = dfs_visit(G, v, time, time_visited, visited)
    
    time_visited[u] = time
    time += 1
    return time

def good_beginning_exists(G):
    n = len(G)
    time_visited = [-1 for _ in range(n)]
    visited = [False for _ in range(n)]

    for i in range(n):
        if not visited[i]:
            time = dfs_visit(G, i, time, time_visited, visited)
    max_v = time_visited.index(n)

    time_visited = [-1 for _ in range(n)]
    visited = [False for _ in range(n)]

    dfs_visit(G, max_v, time, time_visited, visited)

    for val in visited:
        if not val:
            return False
    return True