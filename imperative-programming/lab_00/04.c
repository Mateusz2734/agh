// Dana jest liczba całkowita dodatnia n. 
// Napisz program, który znajduje wszystkie liczby pierwsze 
// mniejsze od n, których cyfry tworzą ciąg niemalejący.

#include <stdio.h>
#include <stdbool.h>

bool isPrime(int n);
bool nonDecreasing(int n);

int main(void) {
    int n;
    printf("Podaj górną granicę ciągu: ");
    scanf("%d", &n);
    for (int i = 1; i < n; i++) {
        if (isPrime(i) && nonDecreasing(i)) {
            printf("%d\n", i);
        }
    }
}

bool isPrime(int n) {
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    for (int i = 3; i*i <= n; i+=2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

bool nonDecreasing(int n) {
    int last = n % 10; 
    n /= 10;
    while (n != 0) {
        if (n % 10 > last) {
            return false;
        }
    n /= 10;
    last %= 10;
    }
    return true;
}