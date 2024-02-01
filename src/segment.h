#include <stdbool.h>

#ifndef SEGMENT_H
#define SEGMENT_H

typedef struct segment {
    int   x1;
    int   y1;
    int   x2; 
    int   y2;
    float slope;
    int   type;
    struct segment *next;
} segment_t;

enum segtype { HOR, VER, UNKNOWN };

segment_t* SEG_get(segment_t *segments, int index);
segment_t* SEG_filter_by_rect( segment_t* obstacles, int r_x1, int r_y1, int r_x2, int r_y2);

int SEG_common_x(segment_t *first, segment_t *second);
int SEG_common_y(segment_t *first, segment_t *second);

bool SEG_contains(segment_t *seg, int x, int y);

void SEG_push(segment_t** head, int x1, int y1, int x2, int y2);
void SEG_free(segment_t* head);
void SEG_delete(segment_t** head, int y);
void SEG_merge(segment_t** head, segment_t* candidates);
void SEG_debug(segment_t* seg);

#endif
