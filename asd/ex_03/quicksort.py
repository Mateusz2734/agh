def partition(T, l, r):
    i = l - 1
    os = T[r]

    for j in range(l, r):
        if T[j] < os:
            i += 1
            T[i], T[j] = T[j], T[i]
    T[i + 1], T[r] = T[r], T[i + 1]
    return i + 1


def quicksort(T, l, r):
    if l < r:
        pop_indeks = partition(T, l, r)

        quicksort(T, l, pop_indeks - 1)
        quicksort(T, pop_indeks + 1, r)
    return T
