# Algorytm dla każdego indeksu zakłada, że jest on środkiem 
# palindromu o nieparzystej długości oraz dla kolejnych wartości k
# sprawdza, czy znaki na indeksach w odległości k od środka są równe
# Złożoność wynosi O(n^2)
from zad1testy import runtests

def ceasar(s):
    n = len(s)
    longest_pal = 1
    for i in range(1,n-1):
        k = 1
        while i - k > 0 and i + k < n:
            if s[i - k] == s[i + k]:
                if longest_pal < 2*k+1:
                    longest_pal = 2*k + 1
                k += 1
            else:
                break
    return longest_pal

# print(ceasar())
# zmien all_tests na True zeby uruchomic wszystkie testy
runtests(ceasar , all_tests = True)
