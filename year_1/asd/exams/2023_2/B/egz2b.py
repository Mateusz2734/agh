# Zamysł to wykorzystanie funkcji z podpowiedzi oraz wykorzystanie
# minimum prefiksowego (odpowiednik sumy prefiksowej, niestety nie znam nazewnictwa) do optymalizacji obliczeń
# Pierwsza pętla (o złożoności O(n)) oblicza odległości pierwszego budynku od możliwych parkingów
# Następne dwie pętle (obie ~ O(m*n)) ustawiają wartość inf do pól, których nie da się osiągnąć
# Ostatnia, dzięki obliczaniu minimum prefiksowego pozwala na utrzymanie złożoności O(m*n)
# Szacowana złożoność czasowa: O(m*n)


from math import inf
from egz2btesty import runtests


def prefix_min(T):
    n = len(T)
    P = [inf for _ in range(n)]

    P[0] = T[0]

    for i in range(1, len(T)):
        P[i] = min(T[i], P[i-1])
    return P


def parking(X, Y):
    n = len(X)
    m = len(Y)
    lookup = [[0 for _ in range(m)] for _ in range(n)]

    for i in range(m-n+1):
        lookup[0][i] = abs(X[0] - Y[i])

    for i in range(n):
        for j in range(m-n+1+i, m):
            lookup[i][j] = inf

    for i in range(1, n):
        for j in range(i):
            lookup[i][j] = inf

    for i in range(1, n):
        mini = prefix_min(lookup[i-1])
        for j in range(i, m-n+1+i):
            lookup[i][j] = mini[j-1] + abs(X[i]-Y[j])
    return min(lookup[n-1])


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(parking, all_tests=True)
