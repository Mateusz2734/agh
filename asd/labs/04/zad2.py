# A: List[int]
# len(A) = n
# dla każdego i  A[i] należy do B oraz len(B) = log(n)
# Złożoność: O(n * log(log n))


def insert(T, x):
    pass


def counting_sort(A):
    B = find_values(A)
    C = [0 for _ in range(len(B))]
    for x in A:
        idx = check(B, x)
        C[idx] += 1

    for i in range(1, len(C)):
        C[i] += C[i - 1]

    res = [0 for _ in range(len(A))]

    for i in range(len(A) - 1, -1, -1):
        idx = check(B, x)
        res[C[idx] - 1] = A[i]
        C[idx] -= 1
    return res


def find_values(tab):
    result = []
    for val in tab:
        idx = check(result, val)
        if idx == -1:
            insert(result, val)
    return result


def check(T, val):
    if not T:
        return -1
    n = len(T)
    left = 0
    right = n - 1
    while left < right:
        x = (right - left) // 2
        if T[x] == val:
            return x
        elif T[x] > val:
            right = x - 1
        else:
            left = x + 1
    return -1


print(check([2, 4, 5, 8, 23, 46, 87], 7))
