# Wstawianie elementu do kopca binarnego
# złożoność czasowa wynosi O(log n)

def parent(index):
    return (index-1)//2

def insert(T, value):
    T.append(value)
    n = len(T) - 1
    par = parent(n)
    while T[par] < T[n]:
        T[par], T[n] = T[n], T[par]
        n = par
