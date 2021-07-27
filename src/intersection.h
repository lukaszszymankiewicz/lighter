#include <stdlib.h>

#ifndef INTERSECTION_H
#define INTERSECTION_H

typedef struct x_intersection {
    int x;
    struct x_intersection* next;
} x_intersection_t;

void INTSC_insert(x_intersection_t** head, int x);
int INTSC_get_last(x_intersection_t* intersections);
void INTSC_free(x_intersection_t* head);

#endif
