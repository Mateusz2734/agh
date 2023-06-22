#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define BUFFER_SIZE 1024
#define MAX_RATE 4
#define MEMORY_ALLOCATION_ERROR  (-1)

typedef union {
	int int_data;
	char char_data;
	// ... other primitive types used
	void *ptr_data;
} data_union;

typedef struct ht_element {
	struct ht_element *next;
	data_union data;
} ht_element;

typedef void (*DataFp)(data_union);
typedef void (*DataPFp)(data_union*);
typedef int (*CompareDataFp)(data_union, data_union);
typedef size_t (*HashFp)(data_union, size_t);
typedef data_union (*CreateDataFp)(void*);

typedef struct {
	size_t size;
	size_t no_elements;
	ht_element *ht;
	DataFp dump_data;
	CreateDataFp create_data;
	DataFp free_data;
	CompareDataFp compare_data;
	HashFp hash_function;
	DataPFp modify_data;
} hash_table;

typedef struct DataWord {
	char *word;
	int counter;
} DataWord;

// calculate hash function for integer k
size_t hash_base(int k, size_t size) {
	static const double c = 0.618033988; // (sqrt(5.) – 1) / 2.;
	double tmp = k * c;
	return (size_t)floor((double)size * (tmp - floor(tmp)));
}

//********************************************
//*******************TASK 0*******************
//********************************************

// return pointer to element with given value
ht_element *get_element(hash_table *p_table, data_union *data) {
	size_t hash = p_table->hash_function(*data, p_table->size);
	ht_element *elem = &p_table->ht[hash];

	if (elem->next == NULL) {
		return NULL;
	}

	elem = elem->next;

	while (elem->next != NULL) {
		if (p_table->compare_data(elem->data, *data) == 0) {
			return elem;
		}
		elem = elem->next;
	}

	if (p_table->compare_data(elem->data, *data) == 0) {
			return elem;
	}

	return NULL;
}

// find element; return pointer to previous
ht_element *find_previous(hash_table *p_table, data_union data) {
	size_t hash = p_table->hash_function(data, p_table->size);
	ht_element *elem = &p_table->ht[hash];

	while (elem->next != NULL) {
		if (p_table->compare_data(elem->next->data, data) == 0) {
			return elem;
		}
		elem = elem->next;
	}

	return NULL;
}

// initialize table fields
void init_ht(hash_table *p_table, size_t size, DataFp dump_data, CreateDataFp create_data,
		DataFp free_data, CompareDataFp compare_data, HashFp hash_function, DataPFp modify_data) {
	p_table->size = size;
	p_table->no_elements = 0;
	p_table->ht = (ht_element*)malloc(size * sizeof(ht_element));
	p_table->dump_data = dump_data;
	p_table->create_data = create_data;
	p_table->free_data = free_data;
	p_table->compare_data = compare_data;
	p_table->hash_function = hash_function;
	p_table->modify_data = modify_data;

	for (size_t i = 0; i < size; i++) {
		p_table->ht[i].next = NULL;
	}
}

// print elements of the list with hash n
void dump_list(const hash_table* p_table, size_t n) {
	ht_element elem = p_table->ht[n];

	if (elem.next == NULL) {
		return;
	} 
	else {
		elem = *(elem.next);
	}
	
	while (elem.next != NULL) {
		p_table->dump_data(elem.data);

		elem = *(elem.next);
	}

	p_table->dump_data(elem.data);
}

// Free element pointed by data_union using free_data() function
void free_element(DataFp free_data, ht_element *to_delete) {
	if (free_data != NULL) free_data(to_delete->data);
	free(to_delete);
}

// free all elements from the table (and the table itself)
void free_table(hash_table* p_table) {
	for (size_t i = 0; i < p_table->size; i++) {
		ht_element *elem = &p_table->ht[i];
		
		if (elem->next == NULL) {
			continue;
		}

		while (elem->next->next != NULL) {
			ht_element *to_delete = elem->next;
			elem->next = to_delete->next;
			free_element(p_table->free_data, to_delete);
		}

		free_element(p_table->free_data, elem->next);
	}

	free(p_table->ht);
}

void rehash(hash_table *p_table) {
	ht_element *new_ht = (ht_element*)malloc(2 * p_table->size * sizeof(ht_element));
	for (size_t i = 0; i < 2 * p_table->size; i++) {
		new_ht[i].next = NULL;
	}
	
	for (size_t i = 0; i < p_table->size; i++) {
		ht_element *elem = &p_table->ht[i];

		if (elem->next == NULL) {
			continue;
		}

		while (elem->next != NULL) {
			ht_element *to_move = elem->next;
			elem->next = to_move->next;
			size_t hash = p_table->hash_function(to_move->data, 2 * p_table->size);
			ht_element *iter = &new_ht[hash];

			to_move->next = iter->next;
			iter->next = to_move;
		}
	}
	
	free_table(p_table);
	p_table->size *= 2;
	p_table->ht = new_ht;
}

// insert element
void insert_element(hash_table *p_table, data_union *data) {
	ht_element *elem = get_element(p_table, data);

	if (elem != NULL && p_table->modify_data != NULL) {
		p_table->modify_data(&elem->data);
		return;
	} else if (elem != NULL && p_table->modify_data == NULL) {
		return;
	}
	
	size_t hash = p_table->hash_function(*data, p_table->size);
	ht_element *to_insert = (ht_element*)malloc(sizeof(ht_element));
	to_insert->data = *data;
	to_insert->next = NULL;
	ht_element *iter = &p_table->ht[hash];

	to_insert->next = iter->next;
	iter->next = to_insert;

	p_table->no_elements++;

	if (p_table->no_elements > 4 * p_table->size) {
		rehash(p_table);
	}
}

// remove element
void remove_element(hash_table *p_table, data_union data) {
	ht_element *to_delete = get_element(p_table, &data);
	ht_element *prev = find_previous(p_table, data);

	if (to_delete == NULL) {
		return;
	}

	if (prev == NULL) {
		p_table->ht[p_table->hash_function(data, p_table->size)].next = to_delete->next;
	} else {
		prev->next = to_delete->next;
	}

	free_element(p_table->free_data, to_delete);

	p_table->no_elements--;
}

//********************************************
//*******************TASK 1*******************
//********************************************

size_t hash_int(data_union data, size_t size) {
	return hash_base(data.int_data, size);
}

void dump_int(data_union data) {
	printf("%d ", data.int_data);
}

int cmp_int(data_union a, data_union b) {
	if (a.int_data == b.int_data) {
		return 0;
	} 

	return 1;
}

data_union create_int(void* value) {
	data_union data;
	
	scanf("%d", &data.int_data);

	if (value != NULL) {
		*(data_union*)value = data;
	}

	return data;
}

//********************************************
//*******************TASK 2*******************
//********************************************

size_t hash_char(data_union data, size_t size) {
	return hash_base((int)data.char_data, size);
}

void dump_char(data_union data) {
	printf("%c ", data.char_data);
}

int cmp_char(data_union a, data_union b) {
	return strcmp(&a.char_data, &b.char_data);
}

data_union create_char(void* value) {
	data_union data;

	scanf(" %c", &data.char_data);
	
	if (value != NULL) {
		*(data_union*)value = data;
	}

	return data;
}

//********************************************
//*******************TASK 3*******************
//********************************************

size_t hash_word(data_union data, size_t size) {
	int s = 0;
	DataWord *dw = (DataWord*)data.ptr_data;
	char *p = dw->word;
	while (*p) {
		s += *p++;
	}
	return hash_base(s, size);
}


void dump_word(data_union data) {
	DataWord *dw = (DataWord*)data.ptr_data;
	printf("%s", dw->word);
}

void free_word(data_union data) {
	DataWord *dw = (DataWord*)data.ptr_data;
	free(dw->word);
	free(dw);
}

int cmp_word(data_union a, data_union b) {
	DataWord *dw_a = (DataWord*)a.ptr_data;
	DataWord *dw_b = (DataWord*)b.ptr_data;

	int cmp = strcmp(dw_a->word, dw_b->word);
	
	if (cmp < 0) {
		return -1;
	} else if (cmp > 0) {
		return 1;
	} else {
		return 0;
	}
}

void modify_word(data_union *data) {
	DataWord *dw = (DataWord*)data->ptr_data;

	dw->counter++;
}

data_union create_data_word(void *value) {
	data_union data;

	DataWord *dw = (DataWord*)malloc(sizeof(DataWord));
	dw->word = (char*)malloc(strlen((char*)value) + 1);
	strcpy(dw->word, (char*)value);
	dw->counter = 1;
	data.ptr_data = dw;

	return data;
}

// read text, parse it to words, and insert these words to the hashtable
void stream_to_ht(hash_table *p_table, FILE *stream) {
	// TODO
}

// test primitive type list
void test_ht(hash_table *p_table, int n) {
	char op;
	data_union data;
	for (int i = 0; i < n; ++i) {
		scanf(" %c", &op);
		p_table->create_data(&data);
		// data = p_table->create_data(NULL); // should give the same result as the line above
		switch (op) {
			case 'r':
				remove_element(p_table, data);
				break;
			case 'i':
				insert_element(p_table, &data);
				break;
			default:
				printf("No such operation: %c\n", op);
				break;
		}
	}
}

int main(void) {
	int to_do, n;
	size_t index;
	hash_table table;
	char buffer[BUFFER_SIZE];
	data_union data;

	scanf ("%d", &to_do);
	switch (to_do) {
		case 1: {
			// test integer hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_int, create_int, NULL, cmp_int, hash_int, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		}

		case 2: {
			// test char hash table
			scanf("%d %zu", &n, &index);
			init_ht(&table, 4, dump_char, create_char, NULL, cmp_char, hash_char, NULL);
			test_ht(&table, n);
			printf ("%zu\n", table.size);
			dump_list(&table, index);
			break;
		} 

		case 3: {
			// read words from text, insert into hash table, and print
			scanf("%s", buffer);
			init_ht(&table, 8, dump_word, create_data_word, free_word, cmp_word, hash_word, modify_word);
			stream_to_ht(&table, stdin);
			printf ("%zu\n", table.size);
			data = table.create_data(buffer);
			ht_element *e = get_element(&table, &data);
			if (e) table.dump_data(e->data);
			if (table.free_data) table.free_data(data);
			break;
		} 

		default: {
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
		}
	}
	free_table(&table);

	return 0;
}
