#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    char *name;
    char *surname;
    char pesel[12];
} Person;

typedef struct PersonNode {
    Person *person;
    struct PersonNode *next;
} PersonNode;

typedef struct {
    PersonNode *Head;
    PersonNode *Tail;
} PersonList;

void init_pl(PersonList *p_pl) {
    p_pl->Head->next = p_pl->Tail;
    p_pl->Tail->next = NULL;
}

void free_pl(PersonList *p_pl) {
    PersonNode *p = p_pl->Head;
    
    while (p != p_pl->Tail) {
        PersonNode *tmp = p;
        p = p->next;
        free(tmp->person->name);
        free(tmp->person->surname);
        free(tmp);
    }
}

void fill_person_data(Person *p_pl, const char *pesel, const char *name, const char *surname) {
    p_pl->name = strdup(name);
    p_pl->surname = strdup(surname);
    
    strcpy(p_pl->pesel, pesel);
}

Person *find(PersonList *p_pl, const char *pesel) {
    PersonNode *p = p_pl->Head;

    while (p != p_pl->Tail) {
        if (strcmp(p->person->pesel, pesel) == 0) {
            return p->person;
        }

        p = p->next;
    }

    return NULL;
}

int add(PersonList *p_pl, const char *pesel, const char *name, const char *surname) {
    Person *foundPerson = find(p_pl, pesel);

    if (foundPerson != NULL) {
        free(foundPerson->name);
        free(foundPerson->surname);
        
        foundPerson->name = strdup(name);
        foundPerson->surname = strdup(surname);
        
        return 1;
    }

    PersonNode *newPersonNode = (PersonNode *)malloc(sizeof(PersonNode));
    newPersonNode->person = (Person*)malloc(sizeof(Person));

    fill_person_data(newPersonNode->person, pesel, name, surname);

    return 0;
}

void merge(PersonList *p_target, PersonList *p_src) {
    PersonNode *p = p_src->Head;

    while (p != p_src->Tail) {
        add(p_target, p->person->pesel, p->person->name, p->person->surname);
        p = p->next;
    }
}

int main() {}