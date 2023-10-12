# Mateusz Wala

# Zamysł algorytmu: przy użyciu algorytmu Dijkstry najpierw liczę koszt drogi z s do t
# Następnie tworzę kopię grafu G (nazwaną L), której krawędzie zawierają koszt po zrabowaniu (dwukrotny + łapówka)
# Później, kolejno iterując wg wierzchołków liczę najmniejszy koszt od s do i na grafie G, dodaję wartość w wierzchołku i,
# czyli V[i] oraz najmniejszy koszt od i do t na grafie L
# Wyliczam i zwracam minimum

from queue import PriorityQueue
from egz1Atesty import runtests
from math import inf


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


def change_graph(n, G, r):
    for i in range(n):
        for j in range(len(G[i])):
            G[i][j] = (G[i][j][0], r + G[i][j][1] * 2)


# G - graf
# V - ilość złota w danym zamku
# s - zamek startowy
# t - zamek końcowy
# r - wysokość łapówki
def gold(G, V, s, t, r):
    n = len(V)

    normal = dijkstra(G, s)

    change_graph(n, G, r)

    changed = dijkstra(G, t)

    min_cost = inf
    for i in range(n):
        cost = normal[i] + changed[i] - V[i]
        if cost < min_cost:
            min_cost = cost
    return min_cost


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(gold, all_tests=True)
