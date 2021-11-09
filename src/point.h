#include <stdlib.h>
#include <stdbool.h>
#include "primitives.h"

#ifndef POINT_H
#define POINT_H

void PT_push(point_t** head, int x, int y);
void PT_free(point_t* head);
point_t* PT_new(int x, int y);
int PT_len(point_t *points);
bool PT_eq(point_t *first, point_t *second);

#endif
