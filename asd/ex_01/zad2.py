# Jednocześnie znaleźć minimum oraz maksimum tablicy
# wykorzystać 3n/2 porównań
def find_min_max(T):
    n = len(T)
    mini = maxi = T[0]
    for i in range(1, n-1, 2):
        if T[i] < T[i+1]:
            if T[i] < mini:
                mini = T[i]
            if T[i+1] > maxi:
                maxi = T[i+1]
        else:
            if T[i] > maxi:
                maxi = T[i]
            if T[i+1] < mini:
                mini = T[i+1]


# posortowana tablica z elem dodatnimi oraz zadajemy liczbę x
# znaleźć i oraz j takie, że T[j] - T[i] = x

def two_sub(T, x):
    n = len(T)
    i = j = 0
    while True:
        if T[j] - T[i] == x:
            return i, j
        if T[j] - T[i] < x:
            j += 1
        else:
            i += 1
        if i == n or j == n:
            return -1, -1


# posortowany ciąg liczb A = {a_0,a_1,...,a_n-1}
# 0 <= a_i <= m-1
# wyrazy są parami różne oraz n<m
# podać najmniejszą liczbę całkowitą, której nie ma w tym ciągu
