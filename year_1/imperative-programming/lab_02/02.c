#include <stdio.h>

#define STACK_SIZE 10

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

int stack[STACK_SIZE];
int top = 0;

int stack_push(int x) {
	if (top == STACK_SIZE) {
		return OVERFLOW;
	} else {
		stack[top] = x;
		top++;
		return OK;
	}
}

int stack_pop(void) {
	if (top == 0) {
		return UNDERFLOW;
	} else {
		int x = stack[top - 1];
		stack[top - 1] = 0;
		top--;
		return x;
	}
}

int stack_state(void) {
	return top;
}

// FIFO queue with shifts

#define QUEUE_SIZE 10

int queue[QUEUE_SIZE];
int in = 0, curr_nr = 0;

void print_arr(int arr[], int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int queue_push(int in_nr) { // in_nr clients try to enter the queue
	for (int i = 1; i <= in_nr; i++) {
		if (in + 1 <= QUEUE_SIZE) {
			queue[in] = curr_nr + 1;
			curr_nr++;
			in++;
		} else {
			curr_nr += in_nr - i + 1;
			return OVERFLOW;
		}	
	}
	return OK;
}

void shift_left(int arr[], int size) {
	for (int i = 1; i < size; i++) {
        arr[i-1] = arr[i];
    }
	arr[size-1] = 0;
}

int queue_pop(int out_nr) {
	for (int i = 0; i < out_nr; i++) {
		if (in - 1 >= 0) {
			shift_left(queue, QUEUE_SIZE);
			in--;
		} else {
			return UNDERFLOW;
		}
	}
	return in;
}

int queue_state(void) {
	return in;
}

void queue_print(void) {
	for (int i = 0; i < QUEUE_SIZE; i++) {
		if (queue[i] != 0) {
			printf("%d ", queue[i]);
		}
	}
}

// Queue with cyclic buffer

#define CBUFF_SIZE 10

int cbuff[CBUFF_SIZE];
int out = 0, len = 0;


int cbuff_push(int cli_nr) {
	int id = (out + len) % CBUFF_SIZE;
	if ( cbuff[id] == 0) {
		cbuff[id] = cli_nr;
		len++;
		return OK;
	} else {
		return OVERFLOW;
	}
}

int cbuff_pop(void) {
	if (len != 0) {
		int p = cbuff[out];
		cbuff[out] = 0;
		out = (out + 1) % CBUFF_SIZE;
		len--;
		return p;
	} else {
		return UNDERFLOW;
	}
	
}

int cbuff_state(void) {
	return len;
}

void cbuff_print(void) {
	for (int i = 0; i < CBUFF_SIZE; i++) {
		if (cbuff[(out + i) % CBUFF_SIZE] != 0) {
			printf("%d ", cbuff[(out + i) % CBUFF_SIZE]);
		}
	}
}

int main(void) {
	int to_do, n, client_no, answer;
	scanf("%d", &to_do);
	switch(to_do) {
		case 1: // stack
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = stack_push(n)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					printf("%d ", stack_pop());
				} else printf("\n%d\n", stack_state());
			} while(n != 0);
			break;
		case 2: // FIFO queue with shifts
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = queue_push(n)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					if ((answer = queue_pop(-n)) < 0) printf("%d ", answer);
				} else {
					printf("\n%d\n", queue_state());
					queue_print();
				}
			} while(n != 0);
			break;
		case 3: // queue with cyclic buffer
			client_no = 0;
			do {
				scanf("%d", &n);
				if (n > 0) {
					if ((answer = cbuff_push(++client_no)) < 0) printf("%d ", answer);
				} else if (n < 0) {
					printf("%d ", cbuff_pop());
				} else {
					printf("\n%d\n", cbuff_state());
					cbuff_print();
				}
			} while(n != 0);
			break;
		default: 
			printf("NOTHING TO DO!\n");
	}
	return 0;
}
