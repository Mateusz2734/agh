#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

enum VehicleType {car=0, bus, truck};
enum EngineType {electric=0, hybrid, combustion};

typedef struct {
    int max_people;
    float engine_power;
} CarSpecific;

typedef struct {
    int sitting_spots;
    int standing_spots;
} BusSpecific;

typedef struct {
    float engine_capacity;
    float max_pressure;
} TruckSpecific;

union VehicleSpecific {
    CarSpecific car;
    BusSpecific bus;
    TruckSpecific truck;
};

typedef struct Vehicle {
    char *owner;
    int inspection_date[3];
    enum VehicleType type;
    enum EngineType engine;
    union VehicleSpecific specific;
} Vehicle;

void new_owner(struct Vehicle *pvehicle) {
    char *owner = (char *)malloc(sizeof(char) * 100);
    printf("Enter new owner: ");
    scanf("%s", owner);
    free(pvehicle->owner);
    pvehicle->owner = owner;
}

int cmp_date(int *date1, int *date2) {
    for (int i = 0; i < 3; i++) {
        if (date1[i] != date2[i])  return 1;
    }
    return 0;
}

int delayed(struct Vehicle *pvehicle, int size, int *base_date, char ***pdelayed_owners) {
    int delayed_count = 0;
    char **delayed_owners = (char **)malloc(sizeof(char *) * size);

    for (int i = 0; i < size; i++) {
        if (cmp_date(pvehicle[i].inspection_date, base_date) != 0) {
            delayed_owners[delayed_count] = pvehicle[i].owner;
            delayed_count++;
        }
    }

    *pdelayed_owners = delayed_owners;

    if (delayed_count == 0) {
        free(delayed_owners);
        pdelayed_owners = NULL;
        return 0;
    }

    delayed_owners = (char **)realloc(delayed_owners, sizeof(char *) * delayed_count);
    *pdelayed_owners = delayed_owners;
    return delayed_count;
}

void print_pojazd(struct Vehicle *pvehicle) {
    printf("Owner: %s\n", pvehicle->owner);
    printf("Inspection date: %d.%d.%d\n", pvehicle->inspection_date[0], pvehicle->inspection_date[1], pvehicle->inspection_date[2]);
    printf("Type: %d\n", pvehicle->type);
    printf("Engine: %d\n", pvehicle->engine);

    switch (pvehicle->type) {
        case car: {
            printf("Max people: %d\n", pvehicle->specific.car.max_people);
            printf("Engine power: %f\n", pvehicle->specific.car.engine_power);
            break;
        }
        case bus: {
            printf("Sitting spots: %d\n", pvehicle->specific.bus.sitting_spots);
            printf("Standing spots: %d\n", pvehicle->specific.bus.standing_spots);
            break;
        }
        case truck: {
            printf("Engine capacity: %f\n", pvehicle->specific.truck.engine_capacity);
            printf("Max pressure: %f\n", pvehicle->specific.truck.max_pressure);
            break;
        }
    }
}

int main() {}