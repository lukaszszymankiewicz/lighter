#include "primitives.h"

#ifndef POINT_H
#define POINT_H

void PT_push(point_t** head, int x, int y);
void PT_free(point_t* head);
void PT_merge( point_t **head, point_t  *candidates);
void PT_debug(point_t *point);

point_t* PT_new(int x, int y);

int PT_len(point_t *points);

bool PT_eq(point_t *first, point_t *second);
bool PT_contains(point_t *head, int x, int y);

#endif
