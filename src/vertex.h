#include "primitives.h"

#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex {
    int x; int y;
    float angle;
    struct vertex* next;
} vertex_t;

float VRTX_calculate_angle(int ax, int ay, int bx, int by);
void VRTX_insert(vertex_t** head, ray_t* ray);
void VRTX_optim(vertex_t* poly);
void VRTX_free(vertex_t* head);

#endif
