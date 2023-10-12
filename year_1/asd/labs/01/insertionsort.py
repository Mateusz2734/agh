def insort(T):
    n = len(T)
    for i in range(1, n):
        tmp = T[i]
        j = i - 1
        while j >= 0 and tmp < T[j]:
            T[j+1] = T[j]
            j -= 1
        T[j+1] = tmp
    return T


print(insort([4, 2, 35, 24, 536, 1, 5, 6,]))
