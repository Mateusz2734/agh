#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(const pair*, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(const pair*, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(const pair*, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(const pair*, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(const pair*, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(const pair*, int);

// Case 2:

// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(const pair*, int);

// A total order relation is a partial order relation that is connected
int is_total_order(const pair*, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(const pair*, int);

int find_max_elements(const pair*, int, int*);
int find_min_elements(const pair*, int, int*);
int get_domain(const pair*, int, int*);
int search_in_relation(const pair*, int, int, int);
void bubble_sort(int *array, int n);
int is_coherent(const pair *relation, int size);
int remove_duplicates(int *array, int n);

// Case 3:

int composition(const pair*, int, const pair*, int, pair*);

// Comparator for pair
int cmp_pair(const void *a, const void *b) {
}

int insert_int(int *tab, int n, int new_element) {
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
	int found = 0;
	for (int i = 0; i < n; i++) {
		if (tab[i].first == new_pair.first && tab[i].second == new_pair.second) {
			found = 1;
		}
	}
	if (!found) {
		tab[n] = new_pair;
		return n+1;
	}
	return n;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
	int n;
	scanf("%d\n", &n);
	for (int i = 0; i < n; i++) {
		scanf("%d %d", &relation[i].first, &relation[i].second);
	}
	return n;
}

void print_int_array(const int *array, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			printf("%d\n", n_domain);
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			printf("%d\n", no_max_elements);
			print_int_array(max_elements, no_max_elements);
			printf("%d\n", no_min_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

int is_reflexive(const pair *relation, int size) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);
	for (int i = 0; i < n; i++) {
		if(!search_in_relation(relation, size, domain[i], domain[i])) {
			return 0;
		}
	}
	return 1;
};

int is_irreflexive(const pair *relation, int size) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);
	for (int i = 0; i < n; i++) {
		if(search_in_relation(relation, size, domain[i], domain[i])) {
			return 0;
		}
	}
	return 1;
};

int is_symmetric(const pair *relation, int size) {
	for (int i = 0; i < size; i++) {
		if (!search_in_relation(relation, size, relation[i].second, relation[i].first)) {
			return 0;
		}
	}
	return 1;
};

int is_antisymmetric(const pair *relation, int size) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (search_in_relation(relation, size, domain[i], domain[j]) && search_in_relation(relation, size, domain[j], domain[i]) && domain[i] != domain[j]) {
				return 0;
			}
		}
	}
	return 1;
};

int is_asymmetric(const pair *relation, int size) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (search_in_relation(relation, size, domain[i], domain[j]) && search_in_relation(relation, size, domain[j], domain[i])) {
				return 0;
			}
		}
	}
	return 1;
};

int is_transitive(const pair *relation, int size) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			for (int z = 0; z < n; z++) {
				if (search_in_relation(relation, size, domain[x], domain[y]) && search_in_relation(relation, size, domain[y], domain[z]) && !search_in_relation(relation, size, domain[x], domain[z])) {
					return 0;
				}
			}
		}
	}
	return 1;
};

int is_partial_order(const pair *relation, int size) {
	return is_reflexive(relation, size) && is_antisymmetric(relation, size) && is_transitive(relation, size);
};

int is_coherent(const pair *relation, int size) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);
	for (int x = 0; x < n; x++) {
		for (int y = 0; y < n; y++) {
			if (!(search_in_relation(relation, size, domain[x], domain[y]) || search_in_relation(relation, size, domain[y], domain[x]))) {
				return 0;
			}
		}
	}
	return 1;
}

int is_total_order(const pair *relation, int size) {
	return is_partial_order(relation, size) && is_coherent(relation, size);
};

int search_int_in_array(const int *array, int n, int element) {
	for (int i = 0; i < n; i++) {
		if (array[i] == element) {
			return 1;
		}
	}
	return 0;
};

int get_domain(const pair *relation, int size, int *domain) {
	int n = 0;
	for (int i = 0; i < size; i++) {
		if (!search_int_in_array(domain, i, relation[i].first)) {
			domain[n] = relation[i].first;
			n++;
		}
		if (!search_int_in_array(domain, i, relation[i].second)) {
			domain[n] = relation[i].second;
			n++;
		}
		
	}
	bubble_sort(domain, n);
	n = remove_duplicates(domain, n);
	return n;
};

int search_in_firsts(const pair *relation, int size, int elem) {
	for (int i = 0; i < size; i++) {
		if (relation[i].first == elem && relation[i].second != elem) {
			return 1;
		}
	}
	return 0;
};

int find_max_elements(const pair *relation, int size, int *max_elements) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);
	int max_len = 0;
	for (int i = 0; i < n; i++) {
		if (!search_in_firsts(relation, size, domain[i])) {
			if (!search_int_in_array(max_elements, max_len, domain[i])) {
				max_elements[max_len] = domain[i];
				max_len++;
			}
		}
	}
	bubble_sort(max_elements, max_len);
	int new_len = remove_duplicates(max_elements, max_len);
	return new_len;
};

int search_in_seconds(const pair *relation, int size, int elem) {
	for (int i = 0; i < size; i++) {
		if (relation[i].second == elem && relation[i].first != elem) {
			return 1;
		}
	}
	return 0;
};

int find_min_elements(const pair *relation, int size, int *min_elements) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);
	int min_len = 0;
	for (int m = 0; m < n; m++) {
		if (!search_in_seconds(relation, size, domain[m])) {
			if (!search_int_in_array(min_elements, min_len, domain[m])) {
				min_elements[min_len] = domain[m];
				min_len++;
			}
		}
	}
	bubble_sort(min_elements, min_len);
	int new_len = remove_duplicates(min_elements, min_len);
	return new_len;
};

int composition(const pair *relation, int size, const pair *relation_2, int size_2, pair *comp_relation) {
	int domain[MAX_REL_SIZE];
	int n = get_domain(relation, size, domain);
	int comp_len = 0;
	for (int i = 0; i < size; i++) {
		for (int y = 0; y < n; y++) {
			if (search_in_relation(relation_2, size_2, relation[i].second, domain[y])) {
				pair new_pair = {relation[i].first, domain[y]};
				comp_len = add_relation(comp_relation, comp_len, new_pair);
			}
		}
	}
	return comp_len;
};

int search_in_relation(const pair *relation, int size, int first, int second) {
	for (int i = 0; i < size; i++) {
		if (relation[i].first == first && relation[i].second == second) {
			return 1;
		}
	}
	return 0;
};

void bubble_sort(int *array, int n) {
	int temp;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - 1; j++) {
			if (array[j] > array[j + 1]) {
				temp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = temp;
			}
		}
	}
};

int remove_duplicates(int *array, int n) {
    if (n == 0 || n == 1)
        return n;
    int temp[n];
    int j = 0;

    for (int i = 0; i < n - 1; i++)
        if (array[i] != array[i + 1])
            temp[j++] = array[i];

    temp[j++] = array[n - 1];

    for (int i = 0; i < j; i++)
        array[i] = temp[i];
    return j;
}