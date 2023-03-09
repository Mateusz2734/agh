def bubblesort(T):
    n = len(T)
    for i in range(n):
        for j in range(n-1):
            if T[j] > T[j+1]:
                T[j], T[j+1] = T[j+1], T[j]
    return T


print(bubblesort([3, 4, 1, 6]))
