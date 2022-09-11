#include <stdbool.h>
#include "primitives.h"

#ifndef VERTEX_H
#define VERTEX_H

vertex_t* VRTX_new(int x, int y, float angle);
void VRTX_free(vertex_t* head);
void VRTX_push(vertex_t** head, vertex_t* new_vertex);
int  VRTX_highest_y(vertex_t* poly);
void VRTX_delete(vertex_t **head, int y);
int VRTX_len(vertex_t *head);
void VRTX_add_point(vertex_t **head, int x, int y, float angle);
bool VRTX_pt_in_segment(int pt_x, int pt_y, int seg_x1, int seg_y1, int seg_x2, int seg_y2);
void VRTX_merge( vertex_t **head, vertex_t  *candidates);
bool VRTX_eq( vertex_t *first, vertex_t *second);

#endif
