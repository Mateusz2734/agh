#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    double *results;
    int len;
} Data;

typedef struct tagQueueElement {
    Data data;
    struct tagQueueElement *next;
} QueueElement;

typedef struct {
    QueueElement *first;
    QueueElement *last;
    int len;
} Queue;

void free_queue(Queue *pqueue) {
    QueueElement *elem = pqueue->first;

    while (elem != NULL) {
        QueueElement *next = elem->next;
        free(elem);
        elem = next;
    }

    pqueue->first = NULL;
    pqueue->last = NULL;
    pqueue->len = 0;
}

void push_copy(Queue *pqueue, const Data *pdata) {
    QueueElement *elem = (QueueElement *)malloc(sizeof(QueueElement));
    elem->data.len = pdata->len;
    elem->data.results = (double *)malloc(sizeof(double) * pdata->len);
    memcpy(elem->data.results, pdata->results, sizeof(double) * pdata->len);
    elem->next = NULL;

    if (pqueue->len == 0) {
        pqueue->first = elem;
        pqueue->last = elem;
        pqueue->len = 1;
        return;
    }

    pqueue->last->next = elem;
    pqueue->last = elem;
    pqueue->len++;
}

int peek(const Queue *pqueue, Data *pdata) {
    if (pqueue->first == NULL || pqueue->len == 0) {
        return 0;
    }

    *pdata = pqueue->first->data;

    return 1;
}

int pop(Queue *pqueue, Data *pdata) {
    if (pqueue->len == 0) return -1;
    if (pqueue->len == 1) {
        *pdata = pqueue->first->data;

        free(pqueue->first);
        pqueue->first = NULL;
        pqueue->last = NULL;
        pqueue->len = 0;
        return 0;
    }

    QueueElement *elem = pqueue->first;
    *pdata = elem->data;
    pqueue->first = elem->next;
    pqueue->len--;
    free(elem);

    return 1;
}

int get_total_count(const Queue *pqueue) {
    QueueElement *elem = pqueue->first;
    int count = 0;

    while (elem != NULL) {
        count += elem->data.len;
        elem = elem->next;
    }

    return count;
}

int main() {}