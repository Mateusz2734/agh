#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
	int day; 
	int month; 
	int year;
} Date;

/*------------------------PART 1--------------------------------*/
// 1 bsearch2

#define FOOD_MAX  30   // max. number of goods
#define ART_MAX   15   // max. length of name's name + 1
#define RECORD_MAX 40  // max. line length

typedef struct {
	char name[20];
	float price;
	int amount;
	Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2) {
	const Date *date1 = (Date*)d1;
	const Date *date2 = (Date*)d2;

	int date1_val = (date1->year * 2000) + (date1->month * 100) + date1->day;
	int date2_val = (date2->year * 2000) + (date2->month * 100) + date2->day;

	if (date1_val > date2_val) {
		return 1;
	} else if (date1_val < date2_val) {
		return -1;
	} else {
		return 0;
	}
}

int cmp_price(const void *p1, const void *p2) {
	const Food *food1 = (Food*)p1;
	const Food *food2 = (Food*)p2;

	if (food1->price > food2->price) {
		return 1;
	} else if (food1->price < food2->price) {
		return -1;
	} else {
		return 0;
	}
}


// compare foods
int cmp(const void *a, const void *b) {
	const Food *food1 = (Food*)a;
	const Food *food2 = (Food*)b;

	int price_cmp = cmp_price(food1, food2);
	int date_cmp = cmp_date(&food1->valid_date, &food2->valid_date);
	if (price_cmp == 0) {
		return date_cmp;
	} else {
		return price_cmp;
	}
}

// bsearch returning address where to insert new element
void *bsearch2 (const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result) {
	int left = 0, right = nmemb - 1, mid;
	while (left <= right) {
		mid = (left + right) / 2;
		int cmp = compar(key, base + mid * size);
		if (cmp == 0) {
			*result = 1;
			return (void*)(base + mid * size);
		} else if (cmp < 0) {
			right = mid - 1;
		} else {
			left = mid + 1;
		}
	}
	*result = 0;
	return (void*)(base + left * size);
}

// print goods of given name
void print_art(Food *p, int n, char *art) {
	for (int i = 0; i < n; i++) {
		if (strcmp(p[i].name, art) == 0) {
			printf("%.2f %d.00 %.2d.%.2d.%d\n", p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
		}
	}
	
}

void print_food(Food *p, int n) {
	for (int i = 0; i < n; i++) {
		printf("%s %.2f %d %d.%d.%d\n", p[i].name, p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
	}
}

// Food *add record(Food *tab, int *np, ComparFp compar, Food *new), która wywołuje funkcję bsearch2() sprawdzającą, czy nowy artykuł (jego dane są zapisane pos adresem *new) jest zapisany w tablicy tab o *np elementach. O tym, czy uznać *new za nowy
// decyduje funkcja wskazywana pointerem do funkcji compar (typu ComparFP – zdefiniowanego w szablonie).
// • Jeżeli *new nie jest elementem nowym, to dane zapisane w elemencie tablicy są modyfikowane danymi zapisanymi w *new – konkretnie – ilość artykułu znalezionego w
// tablicy jest powiększana o ilość zapisaną w *new. Funkcja zwraca adres modyfikowanego elementu tablicy.
// • Jeżeli *new jest elementem nowym, to funkcja add record dodaje we wskazanym
// miejscu nowy element (z ewentualnym przesunięciem części elementów tablicy), zwiększa liczbę elementów tablicy *np i zwraca adres wpisanego elementu.
Food* add_record(Food *tab, int *np, ComparFp compar, Food *new) {
	char result;
	Food *found = bsearch2(new, tab, *np, sizeof(Food), compar, &result);
	if (result) {
		found->amount += new->amount;
		return found;
	} else {
		(*np)++;
		memmove(found + 1, found, (*np - 1) * sizeof(Food));
		*found = *new;
		return found;
	}
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted) {
	int cnt = 0;
	for (int i = 0; i < no; i++) {
		Food new_food;
		fscanf(stream, "%s %f %d %d.%d.%d", new_food.name, &new_food.price, &new_food.amount, &new_food.valid_date.day, &new_food.valid_date.month, &new_food.valid_date.year);
		if (sorted) {
			add_record(tab, &cnt, cmp, &new_food);
			printf("cnt: %d %d\n", cnt, no);
			// cnt += 1;
		} else {
			tab[i] = new_food;
		}
	}
	return cnt;
}

/*------------------------PART 2--------------------------------*/

int cmp_qs(const void *a, const void *b) {
	Food *fa = (Food*)a, *fb = (Food*)b;
	return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b) {
	Date *pd = (Date*)a;
	Food *fb = (Food*)b;
	return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days) {
	//TODO
}

/*------------------------PART 3--------------------------------*/
/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex {F, M};
enum BOOL {no, yes};

struct Bit_data {
	enum Sex sex:1;
	enum BOOL in_line:1;
};

typedef struct {
	char *name;
	struct Bit_data bits;
	Date born;
	char *parent;
} Person;

typedef struct {
	char *par_name;
	int index;
} Parent;

const Date primo_date = { 28, 10, 2011 }; // new succession act

int cmp_primo(Person *person1, Person *person2) {
	if (person1->bits.sex == person2->bits.sex) return 0;
	if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
	if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
	return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
	//TODO
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size) {
	//TODO
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents) {
	//TODO
}

int cleaning(Person *person_tab, int n) {
	//TODO
}

void print_person(const Person *p) {
	printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n) {
	//TODO
}

int main(void) {
	Person person_tab[] = {
		{"Charles III", {M, no}, {14, 11, 1948},"Elizabeth II"},
		{"Anne", {F,yes}, {15, 8, 1950},"Elizabeth II"},
		{"Andrew", {M,yes}, {19, 2, 1960},"Elizabeth II"},
		{"Edward", {M,yes}, {10, 3, 1964} ,"Elizabeth II"},
		{"David", {M,yes}, {3, 11, 1961} ,"Margaret"},
		{"Sarah", {F,yes}, {1, 5, 1964}, "Margaret"},
		{"William", {M,yes}, {21, 6, 1982}, "Charles III"},
		{"Henry", {M,yes}, {15, 9, 1984}, "Charles III"},
		{"Peter", {M,yes}, {15, 11, 1977}, "Anne"},
		{"Zara", {F,yes}, {15, 5, 1981}, "Anne"},
		{"Beatrice", {F,yes}, {8, 8, 1988}, "Andrew"},
		{"Eugenie", {F,yes}, {23, 3, 1990}, "Andrew"},
		{"James", {M,yes}, {17, 12, 2007}, "Edward"},
		{"Louise", {F,yes}, {8, 11, 2003}, "Edward"},
		{"Charles", {M,yes}, {1, 7, 1999}, "David"},
		{"Margarita", {F,yes}, {14, 5, 2002}, "David"},
		{"Samuel", {M,yes}, {28, 7, 1996}, "Sarah"},
		{"Arthur", {M,yes}, {6, 5, 2019}, "Sarah"},
		{"George", {M,yes}, {22, 7, 2013}, "William"},
		{"George VI", {M,no}, {14, 12, 1895}, NULL},
		{"Charlotte", {F,yes}, {2, 5, 2015}, "William"},
		{"Louis", {M,yes}, {23, 4, 2018}, "William"},
		{"Archie", {M,yes}, {6, 5, 2019}, "Henry"},
		{"Lilibet", {F,yes}, {4, 6, 2021}, "Henry"},
		{"Savannah", {F,yes}, {29, 12, 2010}, "Peter"},
		{"Isla", {F,yes}, {29, 3, 2012}, "Peter"},
		{"Mia", {F,yes}, {17, 1, 2014}, "Zara"},
		{"Lena", {F,yes}, {18, 6, 2018}, "Zara"},
		{"Elizabeth II", {F,no}, {21, 4, 1925}, "George VI"},
		{"Margaret", {F,no}, {21, 8, 1930}, "George VI"},
		{"Lucas", {M,yes}, {21, 3, 2021}, "Zara"},
		{"Sienna", {F,yes}, {18, 9, 2021}, "Beatrice"},
		{"August", {M,yes}, {9, 2, 2021}, "Eugenie"}
	};
	int to_do, no;
	int n;
	Food food_tab[FOOD_MAX];
	char buff[ART_MAX];
	fgets(buff, ART_MAX, stdin);
	sscanf(buff, "%d", &to_do);

	switch (to_do) {
		case 1:  // bsearch2
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 1);
			scanf("%s",buff);
			print_art(food_tab, n, buff);
			break;
		case 2: // qsort
			fgets(buff, ART_MAX, stdin);
			sscanf(buff, "%d", &no);
			n = read_goods(food_tab, no, stdin, 0);
			Date curr_date;
			int days;
			scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
			scanf("%d", &days);
			printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
			break;
		case 3: // succession
			scanf("%d",&no);
			int no_persons = sizeof(person_tab) / sizeof(Person);
			create_list(person_tab,no_persons);
			print_person(person_tab + no - 1);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
