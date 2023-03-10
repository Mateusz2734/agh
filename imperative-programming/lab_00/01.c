// Napisz program, który wczytuje liczbę całkowitą N, 
// a następnie oblicza i drukuje na ekran wartość N!
// Uwaga: silnia bardzo szybko powoduje przepełnienie całkowitoliczbowe. 
// Sprawdź dla jakich wartości N
// program wyprowadza prawidłowy wynik. 
// Przetestuj działanie programu dla różnych typów danych 
// (dla ułatwienia można zastosować definicję typedef).

#include <stdio.h>

long long factorial(int n) {
    long long fact = 1;
    for (int i = 2; i <= n; i++) {
        fact *= i;
    }
    return fact;
}

int main() {
    int n;
    scanf("%d", &n);
    printf("%lld\n", factorial(n));
    return 0;
}