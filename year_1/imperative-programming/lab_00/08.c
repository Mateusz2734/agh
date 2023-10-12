// Napisz program tabelaryzujący funkcję cosinus w zadanym przedziale. 
// Dane wejściowe: początek przedziału, koniec przedziału, krok (przyrost x). 
// Tabela powinna zawierać 3 kolumny: wartość argumentu x, wartość cos(x)
// policzoną z wykorzystaniem funkcji bibliotecznej oraz wartość cos(x)
// policzoną z rozwinięcia w szereg Taylora.

#include <stdio.h>
#include <math.h>

double tcos(double x);
long long factorial(int n);

int main (void) {
    double a, b, delta;
    printf("Podaj początek przedziału, koniec przedziału oraz krok: ");
    scanf("%lf %lf %lf", &a, &b, &delta);

    int size = (b - a) / delta;

    printf("    x     cos(x)   tcos(x)\n");
    for (int i = 0; i <= size; i++) {
        double val = a + i*delta;
        double cos_val = cos(val);
        double tcos_val = tcos(val);
        printf("%lf %lf %lf\n", val, cos_val, tcos_val);
    }
    return 0;
}

double tcos(double x) {
    double taylor = 1;
    for (int i = 1; i <= 8; i+=1) {
        int k = 2*i;
        taylor += pow(-1, i) * (pow(x, k) / factorial(k));
    }
    return taylor;
}

long long factorial(int n) {
    long long fact = 1;
    for (int i = 2; i <= n; i++) {
        fact *= i;
    }
    return fact;
}