# Traktor jedzie z punktu A do punktu B. Spalanie traktora to dokładnie jeden litr paliwa na jeden kilometr
# trasy. W baku mieści się dokładnie L litrów paliwa. Trasa z A do B to prosta, na której znajdują się
# stacje benzynowe (na pozycjach będących liczbami naturalnymi; A jest na pozycji 0). Proszę podać
# algorytmy dla następujących przypadków:
#     1. Wyznaczamy stacje na których tankujemy tak, żeby łączna liczba tankowań była minimalna.
#     2. Wyznaczamy stacje tak, żeby koszt przejazdu był minimalny (w tym wypadku każda stacja ma
# dodatkowo cenę za litr paliwa). Na każdej stacji możemy tankować dowolną ilość paliwa.
#     3. j.w., ale jeśli na stacji tankujemy, to musimy zatankować do pełna.


# 1) Bierzemy ostatnią napotkaną stację i na niej tankujemy

def stacje_1(L, S):
    N = len(S)

    lista_stacji = [0]
    poz = L

    while poz < N:
        i = poz
        while i >= poz - L and S[i] is None:
            i -= 1

        lista_stacji.append(i)
        poz += L - (poz - i)

    return lista_stacji

# 2) 
def stacje_2(L, S):
    poz = 0
    N = len(S)

    paliwo = 0
    koszt = 0

    while poz < N:
        najtansza_stacja = sorted([x for x in list(enumerate(S))[poz:poz+L+1] if x[1]], key=lambda x: x[1])[0]
        if najtansza_stacja[0] == poz:
            brakuje = min(L - paliwo, N - poz - paliwo)
            koszt += brakuje * najtansza_stacja[1]
            paliwo += brakuje
        else:
            odl = najtansza_stacja[0] - poz
            ile_dotankowac = max(0, odl - paliwo)
            paliwo += ile_dotankowac
            koszt += ile_dotankowac * najtansza_stacja[1]

# 3) f(x, i) - koszt jazdy od 0 do i, mając x paliwa