#include <stdio.h>

void print_arr(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void swap (int array[], int i, int k) {
	int tmp = array[i];
	array[i] = array[k];
	array[k] = tmp;
}

int bubble_sort(int n, int T[]) {
	for (int i = 0; i < n; i++) {
        print_arr(T, 8);
        int swaps = 0;
        for (int j = 0; j < n-1; j++) {
            if (T[j] > T[j+1]) {
                swaps++;
                swap(T, j, j+1);
            }
        } 
        if (swaps == 0) {
            return i;
        }
    }
}

void shift_left(int arr[], int size) {
	for (int i = 1; i < size; i++) {
        arr[i-1] = arr[i];
    }
    arr[size-1] = 0;
}
