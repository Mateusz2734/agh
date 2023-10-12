#include <stdio.h>
#include <stdlib.h>

enum Vehicle_type {truck, car, v_ndef};

typedef struct {
    float load;
    short axes;
    int trailer_mass;
} Truck_data;

typedef struct {
    char vehicle_ID[15];
    unsigned short places : 3;
    unsigned short driving_wheels : 3;
    unsigned short towbar : 1;
} Car_data;

typedef struct {
    int d;
    int m;
    int y;
} Date;

union Vehicle_union {
    Car_data cs;
    Truck_data ts;
};

typedef struct {
    char owner[30];
    Date valid_date;
    enum Vehicle_type v_type;
    union Vehicle_union veh_union;
} Vehicle;

// Zwraca 1 jeśli delayed, 0 jeśli nie
int cmp_date(const Date *date1, const Date *date2) {
    if (date1->y >= date2->y) return 1;
    if (date1->m >= date2->m) return 1;
    if (date1->d >= date2->d) return 1;

    return 0;
}

int delayed(const Vehicle *tv, int size, const Date *base_date, int **pdelayed_owners) {
    int delayed_count = 0;

    int *delayed_owners = (int *)malloc(sizeof(int) * size);

    // Sprawdzenie prawidłowego przypisania pamięci
    if (delayed_owners == NULL) {
        return -1;
    }
    
    for (int i = 0; i < size; i++) {
        if (cmp_date(&tv[i].valid_date, base_date) == 0) {
            delayed_owners[delayed_count] = i;
            delayed_count++;
        }
    }

    if (delayed_count == 0) {
        free(delayed_owners);
        *pdelayed_owners = NULL;
        return 0;
    }

    delayed_owners = (int *)realloc(delayed_owners, sizeof(int) * delayed_count);
    *pdelayed_owners = delayed_owners;

    return delayed_count;
}

void print_owners(const Vehicle *tv, int *t_ind, size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%s\n", tv[t_ind[i]].owner);
    }
}

Vehicle *create_vehicle(void) {
    Vehicle *v = (Vehicle *)malloc(sizeof(Vehicle));

    if (v == NULL) {
        return NULL;
    }
    
    v->owner[0] = '\0';
    
    v->valid_date.d = 1;
    v->valid_date.m = 1;
    v->valid_date.y = 1970;
    
    v -> v_type = v_ndef;

    return v;
}

void move_date(Vehicle *v, int months) {
    v -> valid_date.m += months;
    if (v -> valid_date.m > 12) {
        v -> valid_date.y += v -> valid_date.m / 12;
        v -> valid_date.m = v -> valid_date.m % 12;
    }
}

void print_v_data(const Vehicle *v) {
    if (v->v_type == v_ndef) return;

    if (v->v_type == truck) {
        printf("Truck: ładowność: %f, liczba osi: %d, masa przyczepy: %d\n", v->veh_union.ts.load, v->veh_union.ts.axes, v->veh_union.ts.trailer_mass);
        return;
    }

    if (v->veh_union.cs.towbar == 0) {
        printf("Car: ID: %s, miejsc: %d, naped na: %d kola, hak: NIE\n", v->veh_union.cs.vehicle_ID, v->veh_union.cs.places, v->veh_union.cs.driving_wheels);
        }
    else {
        printf("Car: ID: %s, miejsc: %d, naped na: %d kola, hak: TAK\n", v->veh_union.cs.vehicle_ID, v->veh_union.cs.places, v->veh_union.cs.driving_wheels);
    }
}

int main() {}