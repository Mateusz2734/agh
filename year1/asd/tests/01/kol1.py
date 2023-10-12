# Mateusz Wala

# Zamysł algorytmu: dla każdego przedziału [0; p-1], ..., [n-p+1; n] znajdujemy k-tą największą liczbę
# przy użyciu algorytmu quickselect i dodajemy ją do sumy

# Szacowana złożoność: O((n-p)*p) -> O(n*p), ponieważ główna pętla zostaje wywołana ~ n razy,
# a quickselect ma złożoność rozmiaru tablicy, czyli w przypadku tego algorytmu: O(p)
from random import randint
from kol1testy import runtests

# k-ty największy element


def partition(T, p, r):
    y = randint(p, r)
    T[y], T[r] = T[r], T[y]
    x = T[r]
    i = p - 1
    for j in range(p, r):
        if T[j] >= x:
            i += 1
            T[i], T[j] = T[j], T[i]
    T[i + 1], T[r] = T[r], T[i + 1]
    return i + 1

# k-ty najw


def quickselect(T, p, r, i):
    if p == r:
        return T[p]
    q = partition(T, p, r)
    k = q - p + 1
    if i == k:
        return T[q]
    elif i < k:
        return quickselect(T, p, q - 1, i)
    else:
        return quickselect(T, q + 1, r, i - k)


def ksum(T, k, p):
    sum = 0
    n = len(T)
    i = 0
    while i < n-p+1:
        sum += quickselect(T[i:i+p], 0, p-1, k)
        i += 1
    return sum


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(ksum, all_tests=True)
