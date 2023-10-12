from collections import deque

#-----BFS WITH ADJACENCY LIST-----
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

# -----BFS WITH ADJACENCY MATRIX-----
def BFS(G, s):
    n = len(G)
    visited = [False for _ in range(n)]
    Q = deque()
    visited[s] = True

    while Q:
        vis = Q.popleft()
        print(vis, end = ' ')
        for i in range(n):
            if (G[vis][i] == 1 and
                    (not visited[i])):
                Q.append(i)
                visited[i] = True
    return visited

# -----DFS WITH ADJACENCY LIST-----
def DFS(G, s):
    visited = [False for _ in range(len(G))]
    result = [s]

    def dfs_visit(u, G, visited, result):
        visited[u] = True 
        for v in G[u]:
            if not visited[v]:
                result.append(v)
                dfs_visit(v, G, visited, result)

    dfs_visit(s, G, visited, result)
    return result