def selectionsort(T):
    n = len(T)
    for i in range(n):
        mini = i
        for k in range(i, n):
            if T[k] < T[mini]:
                mini = k
        T[mini], T[i] = T[i], T[mini]
    return T


print(selectionsort([5, 4, 2, 3, 1]))


def selsort(T):
    n = len(T)
    for i in range(n-1):
        num = i
        for j in range(i+1, n):
            if T[j] < T[num]:
                num = j
        T[num], T[i] = T[i], T[num]
