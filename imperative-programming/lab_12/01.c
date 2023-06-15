#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// list node
typedef struct Node {
	int *data;
	size_t array_size;
	struct Node* next;
	struct Node* prev;
} Node;

// doubly linked list
typedef struct List {
	Node *head;
	Node *tail;
} List;

// iterator
typedef struct iterator {
	struct Node* node_ptr;
	size_t position;
} iterator;

// forward initialization
iterator begin(Node* head) {
	iterator it = { head, 0 };
	return it;
}

// backward initialization;
// points to the element following the last one
iterator end(Node* tail) {
	iterator it = { tail, tail->array_size };
	return it;
}

void *safe_malloc(size_t size) {
	void *ptr = malloc(size);
	if (ptr) return ptr;
	exit(EXIT_FAILURE);
}

void *safe_realloc(void *old_ptr, size_t size) {
	void *ptr = realloc(old_ptr, size);
	if (ptr) return ptr;
	free(old_ptr);
	exit(EXIT_FAILURE);
}

Node *create_node(int *data, size_t array_size, Node *next, Node *prev) {
	// TODO: check if it works properly, if not:
	// Node *node = safe_malloc(sizeof(Node));
	Node *node = (Node *)safe_malloc(sizeof(Node));
	node->data = data;
	node->array_size = array_size;
	node->next = next;
	node->prev = prev;
	return node;
}

// initialize list
// creates the front and back sentinels
void init(List *list) {
	list->head = create_node(NULL, 0, NULL, NULL);
	list->tail = create_node(NULL, 0, NULL, list->head);
	list->head->next = list->tail;
}

// to implement ...

// append node to the list
void push_back(List *list, int *data, size_t array_size) {
	Node* new_node = create_node(data, array_size, list->tail, list->tail->prev);
	list->tail->prev = new_node;
	list->tail->prev->prev->next = new_node;
}

// set iterator to move n elements forward from its current position
void skip_forward(iterator* itr, size_t n) {
	itr->node_ptr = itr->node_ptr->next;
	n -= 1;
	while (n > 0) {
		if (itr->position + n < itr->node_ptr->array_size) {
			itr->position += n;
			n = 0;
		} else {
			n -= itr->node_ptr->array_size - itr->position;
			itr->position = 0;
			itr->node_ptr = itr->node_ptr->next;
		}
	}
}

// forward iteration - get n-th element in the list
int get_forward(List *list, size_t n) {
	iterator itr = begin(list->head);
	skip_forward(&itr, n);
	int nth = itr.node_ptr->data[itr.position];
	return nth;
}

// set iterator to move n elements backward from its current position
void skip_backward(iterator* itr, size_t n) {
	itr->node_ptr = itr->node_ptr->prev;

	while (n > 0) {
		if (itr->node_ptr->array_size < n) {
			n -= itr->node_ptr->array_size;
			itr->node_ptr = itr->node_ptr->prev;
		} else {
			itr->position = itr->node_ptr->array_size - n;
			n = 0;
		}
	}
}

// backward iteration - get n-th element from the end of the list
int get_backward(List *list, size_t n) {
	iterator itr = end(list->tail);
	skip_backward(&itr, n);
	int nth = itr.node_ptr->data[itr.position];
	return nth;
}

void remove_node(Node *node_ptr) {
	node_ptr->next->prev = node_ptr->prev;
	node_ptr->prev->next = node_ptr->next;
	free(node_ptr->data);
	free(node_ptr);
}

// remove n-th element; if array empty remove node
void remove_at(List *list, size_t n) {
	iterator itr = begin(list->head);
	skip_forward(&itr, n);
	if (itr.node_ptr->array_size == 1) {
		remove_node(itr.node_ptr);
	} else {
		int *new_data = (int*)safe_malloc((itr.node_ptr->array_size - 1) * sizeof(int));
		memcpy(new_data, itr.node_ptr->data, itr.position * sizeof(int));
		memcpy(new_data + itr.position, itr.node_ptr->data + itr.position + 1, (itr.node_ptr->array_size - itr.position - 1) * sizeof(int));
		free(itr.node_ptr->data);
		itr.node_ptr->data = new_data;
		itr.node_ptr->array_size -= 1;
	}
}

// return the number of digits of number n
size_t digits(int n) {
	size_t count = 0;
	if (n == 0) return 1;
	while (n != 0) {
		n /= 10;
		count++;
	}
	return count;
}

void insert_node(Node *prev_node, Node *new_node) {
	prev_node->next->prev = new_node;
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	new_node->prev = prev_node;
}

Node *check_if_node_exists(List *list, int value) {
	Node *node = list->head->next;
	while (node != list->tail) {
		if (digits(node->data[0]) == digits(value)) {
			return node;
		}
		node = node->next;
	}
	return NULL;
}

Node *where_to_insert(List *list, int dig) {
	Node *node = list->head->next;
	while (node != list->tail) {
		if (digits(node->data[0]) > (size_t)dig) {
			return node->prev;
		}
		node = node->next;
	}
	return list->tail->prev;
}

int compare(const void *a, const void *b) {
	return (*(int*)a - *(int*)b);
}

void sort_list(List *list) {
	Node *node = list->head->next;
	while (node != list->tail) {
		qsort(node->data, node->array_size, sizeof(int), compare);
		node = node->next;
	}
}

// inserts 'value' to the node with the same digits' count
// otherwise insert new node
void put_in_order(List *list, int value) {
	Node *node = check_if_node_exists(list, value);
	if (node == NULL) {
		node = where_to_insert(list, digits(value));
		int *data = (int*)safe_malloc(sizeof(int));
		data[0] = value;
		Node *new_node = create_node(data, 1, NULL, NULL);
		insert_node(node, new_node);
	} else {
		int *data = (int*)safe_realloc(node->data, (node->array_size + 1) * sizeof(int));
		data[node->array_size] = value;
		node->data = data;
		node->array_size += 1;
	}

	sort_list(list);
}

// -------------------------------------------------------------
// helper functions

// print list
void dumpList(const List *list) {
	for(Node *node = list->head->next; node != list->tail; node = node->next) {
		printf("-> ");
		for (size_t k = 0; k < node->array_size; k++) {
			printf("%d ", node->data[k]);
		}
		printf("\n");
	}
}

// free list
void freeList(List *list) {
	Node *to_delete = list->head->next, *next;
	while(to_delete != list->tail) {
		next = to_delete->next;
		remove_node(to_delete);
		to_delete = next;
	}
}

// read int vector
void read_vector(int tab[], size_t n) {
	for (size_t i = 0; i < n; ++i) {
		scanf("%d", tab + i);
	}
}

// initialize the list and push data
void read_list(List *list) {
	int n;
	size_t size;
	scanf("%d", &n); // number of nodes
	for (int i = 0; i < n; i++) {
		scanf("%zu", &size); // length of the array in i-th node
		int *tab = (int*) safe_malloc(size * sizeof(int));
		read_vector(tab, size);
		push_back(list, tab, size);
	}
}

int main() {
	int to_do, value;
	size_t size, m;
	List list;
	init(&list);

	scanf("%d", &to_do);
	switch (to_do) {
		case 1:
			read_list(&list);
			dumpList(&list);
			break;
		case 2:
			read_list(&list);
			scanf("%zu", &size);
			for (size_t i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_forward(&list, m));
			}
			printf("\n");
			break;
		case 3:
			read_list(&list);
			scanf("%zu", &size);
			for (size_t i = 0; i < size; i++) {
				scanf("%zu", &m);
				printf("%d ", get_backward(&list, m));
			}
			printf("\n");
			break;
		case 4:
			read_list(&list);
			scanf("%zu", &size);
			for (size_t i = 0; i < size; i++) {
				scanf("%zu", &m);
				remove_at(&list, m);
			}
			dumpList(&list);
			break;
		case 5:
			scanf("%zu", &size);
			for (size_t i = 0; i < size; i++) {
				scanf("%d", &value);
				put_in_order(&list, value);
			}
			dumpList(&list);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	freeList(&list);

	return 0;
}
