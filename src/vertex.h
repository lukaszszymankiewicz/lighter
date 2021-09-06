#include "primitives.h"

#ifndef VERTEX_H
#define VERTEX_H

void VRTX_insert(vertex_t** head, ray_t* ray);
void VRTX_optim(vertex_t* poly);
void VRTX_free(vertex_t* head);
void VRTX_complete_vertex(vertex_t* vertices);
void VRTX_push(vertex_t** head, vertex_t* new_vertex);

#endif
