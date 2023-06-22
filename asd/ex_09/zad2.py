# Problem pokrycia przedziałami jednostkowymi
# Dany jest zbiór punktów X = {x[1], ..., x[n]} na prostej. Proszę podać algorytm, który znajduje
# minimalną liczbę przedziałów jednostkowych domkniętych, potrzebnych do pokrycia wszystkich punktów z X.
# (Przykład: Jeśli X = {0.25, 0.5, 1.6} to potrzeba dwóch przedziałów, np. [0.2, 1.2] oraz [1.4, 2.4]).

# Sortujemy punkty rosnąco

def przedziały(P):
    P.sort()

    licznik = 0

    while P:
        start = P[0]
        while start <= P[0] <= start + 1:
            P.pop(0)
        licznik += 1
    return licznik

def przedziały_reverse(P):
    P.sort(reversed=True)
    licznik = 0

    while P:
        start = P[-1]
        while start <= P[-1] <= start + 1:
            P.pop()

        licznik += 1
    return licznik