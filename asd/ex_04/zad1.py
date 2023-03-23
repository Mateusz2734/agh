# Posortować T[int] <0; n^2-1>
# Z użyciem CountingSorta

# Pomysł: zapisujemy liczby w systemie o podstawie n
# Używamy RadixSorta

def change(x, n, i):
    wynik = 0
    for j in range(i-1):
        x //= n
    return x % n

def change_2(x, n, i): 
    if i == 1:
        return x % n
    else:
        return (x // n) % n

def radix_sort(T):
    n = len(T)
    for i in range(1, 3):
        T = counting_sort(T, n, i)
    return T

def counting_sort(T, n, i):
    x = len(T)
    Tmp = [0 for _ in range(n)]
    T2 = [0 for _ in range(x)]

    for j in range(x):
        Tmp[change_2(T[j], n, i)] += 1

    for j in range(1, n):
        Tmp[j] += Tmp[j-1]

    for j in range(x-1, -1, -1):
        T2[Tmp[change_2(T[i], n, i)]-1] = T[j]
        Tmp[change(T[j], n, i)] -= 1
    return T2

print(radix_sort([8,5,7]))