class Node:
    def __init__(self, val, next=None):
        self.val = val
        self.next = next


def insert_node(L: Node, p: Node):
    while L.next and L.next.val < p.val:
        L = L.next
    p.next = L.next
    L.next = p


def list_insort(L):
    head = L
    while L.next:
        tmp = L.next
        L.next = L.next.next
        tmp.next = None
        insert_node(head, tmp)

# Dodać selsort
