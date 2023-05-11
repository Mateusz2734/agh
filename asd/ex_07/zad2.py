from copy import deepcopy
# Kilka źródeł i kilka ujść z pewnymi możliwościami wytwórczymi
# Robimy jedno uniwersalne źródło i jedno uniwersalne ujście i "podłączamy" je do odpowiednio źródeł lub ujść, 
# następnie odpalamy algorytm znajdowania maksymalnego przepływu


def modify_graph(M, sources, sinks):
    G = deepcopy(M)
    n = len(G)

    for _ in range(2):
        G.append([0 for _ in range(n)])

    for i in range(n+2):
        G[i].extend([0,0])

    # źródło ma indeks n, ujście ma indeks n+1
    for v, w in sources:
        G[n][v] = w

    for v, w in sinks:
        G[v][n+1] = w

    return G
    # Ewentualnie return ford_fulkerson(G, n, n+1)