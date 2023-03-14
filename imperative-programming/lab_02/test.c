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

int main(void) {
    int arr1[8] = {0, 1, 2, 3, 7, 4, 5, 6};
    int arr2[8] = {1, 2, 3, 7, 4, 5, 6, 0};
    int arr3[8] = {0, 1, 2, 3, 4, 5, 6, 7};
    
    int a = bubble_sort(8, arr1);
    print_arr(arr1, 8);
    int b = bubble_sort(8, arr2);
    int c = bubble_sort(8, arr3);
    printf("%d %d %d", a, b, c);
    return 0;
}