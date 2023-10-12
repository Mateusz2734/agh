from math import inf
from egz1btesty import runtests

# D - odległości i-tych planet od planety A
# C - cena tony paliwa na i-tej planecie
# T - pary (numer planety na którą można się teleportować, koszt teleportu)
# E - pojemność zbiornika paliwa
def planets(D, C, T, E):
    n = len(D)

    F = [[inf for _ in range(E+1)] for _ in range(n)]

    for j in range(E+1):
        F[0][j] = j * C[0]

    for i in range(n):
        for j in range(E+1):
            for k in range(E, 0, -1):
                if k > D[i]-D[i-1]:
                    odl = D[i]-D[i-1]
                    F[i][j] = min(F[i-1][k] + C[i]*(j-(k-odl)), F[i][j])

            for l in range(n):
                if T[l][0] == i:
                    F[i][j] = min(F[i][0] + T[l][1] + j*C[i], F[i][j])

    return min(F[n-1])

# Minimalny koszt znalezienia się na planecie i mając b ton paliwa


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(planets, all_tests=False)
