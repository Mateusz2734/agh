#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define NR_CARDS 52
#define CBUFF_SIZE 52
#define HALF_DECK 26

enum state { OK = -1, UNDERFLOW = -2, OVERFLOW = -3 };
int deck[NR_CARDS];
int table_A[NR_CARDS];
int table_B[NR_CARDS];
int player_A_cards[CBUFF_SIZE];
int player_B_cards[CBUFF_SIZE];
int len_A = 0, len_B = 0;
int out_A = 0, out_B = 0;
int conflicts = 0, undecided = 0;

void cbuff_print(int out, int cbuff[]);
int cbuff_A_push(int card_nr);
int cbuff_B_push(int card_nr);
int cbuff_A_pop(void);
int cbuff_B_pop(void);
void print_arr(int arr[], int n);
int get_power(int card);
int rand_from_interval(int a, int b);
void swap (int array[], int i, int k);
void rand_permutation(int n, int array[]);
void fill_player_cards(void);
void init_tables(void);
void mode_1_logic(int A, int B);

void mode_1_logic(int A, int B) {
    if(get_power(A) > get_power(B)) {
        cbuff_A_push(A);
        cbuff_A_push(B);
    } else if (get_power(A) < get_power(B)) {
        cbuff_B_push(A);
        cbuff_B_push(B);
    } else {
        cbuff_A_push(A);
        cbuff_B_push(B);
    }
}

int main(void) {
    int seed, mode, max_conflicts;
    
    // Set values from user input
    scanf("%d %d %d", &seed, &mode, &max_conflicts);
    srand((unsigned) seed);

    // Initialize deck and cards on table
    rand_permutation(NR_CARDS, deck);
    init_tables();
    fill_player_cards();

    while (len_A != 0 && len_B != 0 && conflicts < max_conflicts) {
        int A = cbuff_A_pop();
        int B = cbuff_B_pop();
        if (mode == 1) {
            conflicts++;
            mode_1_logic(A, B);
        }
    }

    if (conflicts >= max_conflicts) {
        printf("0 ");
        printf("%d\n", len_A);
        printf("%d\n", len_B);
    } //else if () {} 
    else if (len_B == 0) {
        printf("2 ");
        printf("%d\n", conflicts);
    } else if (len_A == 0) {
        printf("3\n");
        cbuff_print(out_B, player_B_cards);
    }


    // print_arr(deck, 52);
    // printf("\n-------------------\n");
    // cbuff_print(out_A, player_A_cards);
    // printf("\n-------------------\n");
    // cbuff_print(out_B, player_B_cards);
}

void init_tables(void) {
    for (int i = 0; i < NR_CARDS; i++) {
        table_A[i] = -1;
        table_B[i] = -1;
    }
}

void fill_player_cards(void) {
    for (int i = 0; i < HALF_DECK; i++) {
        cbuff_A_push(deck[i]);
        cbuff_B_push(deck[i+HALF_DECK]);
    }
}

// get "power" of a card
int get_power(int card) {
    return card >> 2;
}

// return random number from [a, b]
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

// swap elements of given array
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

void print_arr(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

// CBUFF SECTION
int cbuff_A_push(int card_nr) {
	int id = (out_A + len_A) % CBUFF_SIZE;
	if ( player_A_cards[id] == 0) {
		player_A_cards[id] = card_nr;
		len_A++;
		return OK;
	} else {
		return OVERFLOW;
	}
}

int cbuff_B_push(int card_nr) {
	int id = (out_B + len_B) % CBUFF_SIZE;
	if ( player_B_cards[id] == 0) {
		player_B_cards[id] = card_nr;
		len_B++;
		return OK;
	} else {
		return OVERFLOW;
	}
}

int cbuff_A_pop(void) {
	if (len_A != 0) {
		int p = player_A_cards[out_A];
		player_A_cards[out_A] = 0;
		out_A = (out_A + 1) % CBUFF_SIZE;
		len_A--;
		return p;
	} else {
		return UNDERFLOW;
	}	
}

int cbuff_B_pop(void) {
	if (len_B != 0) {
		int p = player_B_cards[out_B];
		player_B_cards[out_B] = 0;
		out_B = (out_B + 1) % CBUFF_SIZE;
		len_B--;
		return p;
	} else {
		return UNDERFLOW;
	}	
}


void cbuff_print(int out, int cbuff[]) {
	for (int i = 0; i < CBUFF_SIZE; i++) {
		if (cbuff[(out + i) % CBUFF_SIZE] != 0) {
			printf("%d ", cbuff[(out + i) % CBUFF_SIZE]);
		}
	}
}
