# Sprawdzić, czy dwa słowa są anagramami
# O(n)


# 1. Dla małego k:
def count(A, B, k):
    n = len(A)
    cnt = [0 for _ in range(k)]
    for i in range(n):
        cnt[A[i]] += 1
        cnt[B[i]] -= 1

    for x in cnt:
        if x != 0:
            return False
    return True

print(count([1,5,4,2], [1,2,2,5], 10))
# 2. Dla dużego k:
# get_empty_table(k)
