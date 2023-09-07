# Zamysł algorytmu:
# Dla każdego punktu iterujemy po wszystkich innych punktach i sprawdzamy ile punktów on dominuje
# Następnie zwracamy maksimum

from egz2atesty import runtests


def dominance(P):
    n = len(P)
    max_cnt = 0
    for i in range(n):
        curr_cnt = 0
        for j in range(n):
            if i == j:
                continue
            if P[i][0] > P[j][0] and P[i][1] > P[j][1]:
                curr_cnt += 1
        max_cnt = max(max_cnt, curr_cnt)
    return max_cnt


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(dominance, all_tests=True)
