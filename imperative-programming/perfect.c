#include <stdio.h>

int sumOfDivisors(int n);


int main() {
    long start, end;
    int cnt = 0;
    scanf("%ld %ld", &start, &end);
    for (long i = start; i <= end; i++) {
        if (sumOfDivisors(i) == i) {
            cnt += 1;
            printf("%ld ", i);
        }
    }
    printf("\n%d", cnt);
    return 0;
}

int sumOfDivisors(int n) {
    int sum = 0;
    for (int i = 1; i<=n/2; i++) {
        if (n % i == 0) {
            sum += i;
        }
    }
    return sum;
}