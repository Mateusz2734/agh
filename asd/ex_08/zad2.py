# Mamy listę przedziałów (klocków) postaci [a, b]. Dany jest ciąg klocków [a_1, b_1], [a_2, b_2], ..., [a_n, b_n]
# Spadają one na oś liczbową.
# Zaproponować algorytm, który oblicza ile klocków usunąć z listy, żeby każdy kolejny spadający klocek
# mieścił się w całości w tym, który spadł tuż przed nim

# f(i) = min( f(i-1) + 1, min j<i(f(j)+(i-j+1))(a_i,b_i)<=g(j), i )
# f(0) = 0


def klocki(T, cache, top, i):
    if i == 0:
        top[0] = T[0]
        return 0

    if cache[i] != -1:
        return cache[i]

    cache[i] = min(
        klocki(T, cache, top, i - 1) + 1,
        min(
            [
                klocki(T, cache, top, j) + (i - j + 1)
                for j in range(i)
                if upper(T[i], top[j])
            ]
        ),
        i,
    )

    if cache[i] == klocki(T, cache, top, i - 1) + 1:
        top[i] = top[i - 1]
    else:
        top[i] = T[i]
    return cache[i]


def upper(a, b):
    return not (a[0] < b[0] or a[1] > b[1])


def klocki(T, n):
    cache = [-1 for _ in range(n)]
    top = [None for _ in range(n)]
    cache[0] = 0
    top[0] = T[0]
    return klocki(T, cache, top, n)
