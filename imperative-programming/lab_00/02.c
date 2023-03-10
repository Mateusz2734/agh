// Napisz program, który dla zadanej liczby naturalnej n odpowiada
// na pytanie, czy liczba ta jest iloczynem dowolnych dwóch kolejnych 
// wyrazów ciągu Fibonacciego. 
// Zakładamy, że pierwsze dwa wyrazy ciągu Fibonacciego to 0 i 1.

# include <stdio.h>

int main(void) {
    int x, tmp;
    int a = 0;
    int b = 1;
    int il;
    printf("Podaj liczbę do sprawdzenia: ");
    scanf("%d", &x);

    while (1) {
        tmp = a;
        a = b;
        b = tmp + b;
        il = a*b;
        if (il == x || x == 0) {
            printf("Liczba %d jest iloczynem dwoch kolejnych wyrazow ciagu fib\n", x);
            break;
        } else if (il > x) {
            printf("Liczba %d nie jest iloczynem dwoch kolejnych wyrazow ciagu fib\n", x);
            break;
        }
    }


    return 0;
}