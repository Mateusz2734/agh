# Wypisać najkrótszą ścieżkę na podstawie listy parentów

# def print_path(start, u, parent):
#     if u == start:
#         print(u)
#     else:
#         print_path(start, parent[u], parent)
#         print(u)

def print_path(start, u, parent):
    if start != u:
        print_path(start, parent[u], parent)
    print(u)

def print_path_iter(start, u, parent):
    path = [u]
    while u != start:
        u = parent[u]
        path.append(u)
    for i in range(len(path)-1, -1, -1):
        print(path[i])