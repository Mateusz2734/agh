from kolutesty import runtests


def ice_cream(T: list):
    T.sort(reverse=True)

    vol = 0

    for i in range(len(T)):
        if T[i]-i > 0:
            vol += T[i]-i

    return vol


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(ice_cream, all_tests=True)
