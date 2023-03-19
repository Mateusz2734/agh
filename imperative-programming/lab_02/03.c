#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NR_CARDS 52
#define CBUFF_SIZE 52

void print_arr(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int get_power(int card);
int rand_from_interval(int a, int b);
void swap (int array[], int i, int k);
void rand_permutation(int n, int array[]);

int main(void) {
    int seed, mode, max_conflicts;
    int deck[NR_CARDS];

    scanf("%d, %d, %d", &seed, &mode, &max_conflicts);
    srand((unsigned) seed);
    rand_permutation(NR_CARDS, deck);
    print_arr(deck, 52);
}

int get_power(int card) {
    return card >> 2;
}

int rand_from_interval(int a, int b) {
	if (a > b) {
		return INT_MIN;
	} else if (b - a > RAND_MAX) {
		return INT_MAX;
	} else if (a == b) {
		return a;
	} else {
		return rand()%(b + 1 - a) + a;
	}
}

void swap (int array[], int i, int k) {
	int tmp = array[i];
	array[i] = array[k];
	array[k] = tmp;
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
	for (int i = 0; i < n; i++) {
		array[i] = i;
	}	
	for (int i = 0; i < n - 1; i++) {
		int k = rand_from_interval(i, n-1);
		swap(array, i, k);
	}
}