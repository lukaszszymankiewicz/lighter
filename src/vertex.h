#include "global.h"
#include "primitives.h"

#ifndef VERTEX_H
#define VERTEX_H

vertex_t* VRTX_new(int x, int y, float angle);
vertex_t* VRTX_transpose(vertex_t *vertex, int x_corr, int y_corr);

int VRTX_highest_y(vertex_t* poly);
int VRTX_len(vertex_t *head);
int VRTX_pop_x(vertex_t* vertex);
int VRTX_pop_y(vertex_t* vertex);

void VRTX_free(vertex_t* head);
void VRTX_push(vertex_t** head, vertex_t* new_vertex);
void VRTX_delete(vertex_t **head, int y);
void VRTX_add_point(vertex_t **head, int x, int y, float angle);
void VRTX_merge(vertex_t **head, vertex_t  *candidates);
void VRTX_debug(vertex_t *vertex);
void VRTX_delete(vertex_t **head, int y);
void VRTX_merge_unique(vertex_t **head, vertex_t *candidates);

bool VRTX_pt_in_segment(int pt_x, int pt_y, int seg_x1, int seg_y1, int seg_x2, int seg_y2);
bool VRTX_eq(vertex_t *first, vertex_t *second);

#endif
