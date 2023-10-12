# Mamy k posortowanych linked list
# Jak najszybsze scalenie w jedną listę

def merge(L):
    build_min_heap(L)
    a = Node()
    last = a
    while L:
        last.next = L[0]
        L[0] = L[0].next
        last = last.next
        last.next = None
        L[0], L[-1] = L[-1], L[0]
        if L[-1] is None:
            L.pop()
        heapify(L, 0)
    return a.next