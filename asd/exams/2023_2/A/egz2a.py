# Zamysł algorytmu:
# Zauważamy, że punktem o największej dominacji, będzie punkt, którego iloczyn współrzędnej x oraz y będzie największy
# Dlatego też liniowo wyszukujemy maksymalnego iloczynu współrzędnych oraz zapisujemy je w osobnych zmiennych
# Następnie, również liniowo, dla każdego punktu sprawdzamy czy jego współrzędne są mniejsze od maksymalnych współrzędnych.
# Jeśli tak, to inkrementujemy licznik. Na koniec zwracamy licznik
# Szacowane złożoności:
# Czasowa O(n), ponieważ korzystam jedynie z dwóch liniowych pętli
# Pamięciowa O(1), ponieważ nie zapamiętuję żadnych tablic

from egz2atesty import runtests


def dominance(P):
    max_il = 0
    max_x = 0
    max_y = 0
    for x, y in P:
        if x*y > max_il:
            max_il = x*y
            max_x = x
            max_y = y

    cnt = 0
    for x, y in P:
        if x < max_x and y < max_y:
            cnt += 1

    return cnt


# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(dominance, all_tests=True)
