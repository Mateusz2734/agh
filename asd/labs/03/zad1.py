# Quicksort, ale max. złożoność pamięciowa O(log n)

def partition(T, left, right):
    if left >= right:
        return left
    pivot = T[right]
    j = left
    for i in range(left,right):
        if T[i] < pivot:
            T[i], T[j] = T[j], T[i]
            j += 1
    T[right], T[j] = T[j], T[right]
    return j

def quicksort(T, left, right):
    while left < right:
        pivot = partition(T, left, right)
        if (right - pivot > pivot - left):
            quicksort(T, left, pivot-1) #(T, left, pivot-1)
            left = pivot + 1
        else:
            quicksort(T, pivot+1, right) #(T, pivot+1, right)
            right = pivot - 1
    return T

print(quicksort([5, 18, 44, 24, 63, 6, 3, 42, 43],0,8))