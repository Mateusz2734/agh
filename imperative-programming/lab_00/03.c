// Liczba doskonała jest to taka liczba naturalna, która jest
// sumą wszystkich swych dzielników właściwych (to znaczy 
// od niej mniejszych). Najmniejszą liczbą doskonałą jest 6, 
// ponieważ jej dzielnikami właściwymi są 1, 2, 3 i 1 + 2 + 3 = 6. 
// Napisz program, który znajduje wszystkie liczby doskonałe
// w zadanym przedziale oraz ich liczbę.

#include <stdio.h>

int sumOfDivisors(int n) {
    int sum = 0;
    for (int i = 1; i<=n/2; i++) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return sum;
}

int main(void) {
    int down, up;
    int cnt = 0;
    scanf("%d", &down);
    scanf("%d", &up);
    for (int i = down; i < up; i++) {
        if (sumOfDivisors(i) == i) {
            cnt += 1;
            printf("%d ", sumOfDivisors(i));
        }
    }
    printf("\n%d\n", cnt);
    return 0;
}