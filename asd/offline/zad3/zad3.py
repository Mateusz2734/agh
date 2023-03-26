# Mateusz Wala
# Zamysl algorytmu: iterujemy po kazdym elemencie wejściowej tablicy i sprawdzamy czy jest palindromem. 
# Jesli nie jest, to dodajemy do tablicy T2 jego odwrocona wersje. Nastepnie laczymy tablice T i T2 i sortujemy je 
# algorytmem quicksort z wyborem mediany z 3 liczb: lewej granicy, prawej granicy i losowej liczby z przedzialu.
# Na koncu znajdujemy dlugosc najdluzszego podciagu o wyrazach stalych, czyli sile najsilniejszego slowa
# Szacowana zlozonosc: O(N + 2n*log(2n)) czyli O(N + n*log(n))
import random
from zad3testy import runtests 

def med(a, b, c):
    if (b < a and a < c) or (c < a and a < b):
        return a
    if (a < b and b < c) or (c < b and b < a):
        return b
    else:
        return c

# Partition z wyborem mediany z 3 liczb: lewej granicy, prawej granicy i losowej liczby z przedzialu
def partition(T, l, r):
    pivot = med(l, random.randint(l, r), r)
    T[l], T[pivot] = T[pivot], T[l]
    i = l - 1
    x = T[r]

    for j in range(l, r):
        if T[j] < x:
            i += 1
            T[i], T[j] = T[j], T[i]
    T[i + 1], T[r] = T[r], T[i + 1]
    return i + 1

def quicksort(T, l, r):
    if l < r:
        pivot = partition(T, l, r)
        quicksort(T, l, pivot - 1)
        quicksort(T, pivot + 1, r)
    return T

# Funkcja znajduje dlugosc najdluzszego podciagu o wyrazach stalych
def find_lcs(T):
    max = 0
    count = 0
    for i in range(len(T)-1):
        if T[i] == T[i+1]:
            count += 1
        else:
            if count > max:
                max = count
            count = 0
    return max + 1

def is_palindrome(s):
    return s == s[::-1]

def strong_string(T):
    T2 = []
    for elem in T:
        if not is_palindrome(elem):
            T2.append(elem[::-1])
    T = T + T2
    quicksort(T, 0 , len(T)-1)
    return find_lcs(T)

# # zmien all_tests na True zeby uruchomic wszystkie testy
runtests( strong_string, all_tests=True )
