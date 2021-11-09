#include <stdbool.h>
#include "primitives.h"

#ifndef VERTEX_H
#define VERTEX_H

vertex_t* VRTX_new(int start_x, int start_y, int x, int y);
void VRTX_optim(vertex_t* poly);
void VRTX_free(vertex_t* head);
void VRTX_push(vertex_t** head, vertex_t* new_vertex);
int  VRTX_highest_y(vertex_t* poly);
void VRTX_delete(vertex_t **head, int y);
int VRTX_len(vertex_t *head);
float VRTX_calculate_angle(int ax, int ay, int bx, int by);
void VRTX_add_point(vertex_t **head, int x, int y, float angle);
bool VRTX_pt_in_segment(int pt_x, int pt_y, int seg_x1, int seg_y1, int seg_x2, int seg_y2);

#endif
