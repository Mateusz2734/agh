# quicksort iteracyjnie

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

def iter_quicksort(T):
    stack = []
    stack.append((0, len(T) - 1))
    while len(stack) > 0: # while stack
        left, right = stack.pop()
        if right - left > 0:
            pivot = partition(T, left, right)
            stack.append((left, pivot -1))
            stack.append((pivot+1, right))
