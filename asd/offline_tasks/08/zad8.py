from zad8testy import runtests
from math import inf
from queue import PriorityQueue


def solve(T):
    n = len(T)
    Q = PriorityQueue()

    jumps = 1

    if T[0] >= n - 1:
        return 1

    for i in range(T[0]):
        if T[i] > 0:
            Q.put((-T[i], i))

    while not Q.empty():
        fuel, pos = Q.get()
        fuel *= -1
        if pos + fuel >= n - 1:
            return jumps
        for i in range(pos + fuel, pos, -1):
            if T[i] > 0:
                Q.put((-T[i], i))
        jumps += 1
    return jumps


def dfs(M, i, j, visited):
    m = len(M)
    n = len(M[0])

    if i < 0 or j < 0 or i > m - 1 or j > n - 1 or M[i][j] == 0 or visited[i][j]:
        return 0, visited

    visited[i][j] = True

    suma = 0
    suma += dfs(M, i - 1, j, visited)[0]
    suma += dfs(M, i + 1, j, visited)[0]
    suma += dfs(M, i, j - 1, visited)[0]
    suma += dfs(M, i, j + 1, visited)[0]

    return suma + M[i][j], visited


def calculate_fuel(T):
    m = len(T)
    n = len(T[0])
    visited = [[False for _ in range(n)] for _ in range(m)]

    for j in range(n):
        if T[0][j] != 0 and not visited[0][j]:
            T[0][j], visited = dfs(T, 0, j, visited)
        else:
            T[0][j] = 0
    return T[0]


def plan(T):
    fuel_spots = calculate_fuel(T)

    return solve(fuel_spots)


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(plan, all_tests=True)
