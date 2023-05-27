#include <stdbool.h>

#include "vertex.h"

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

segment_t* SEG_init(int x1, int y1, int x2, int y2);
segment_t* SEG_get_segments_of_polygon(vertex_t *poly);
segment_t* SEG_find_candidates(segment_t** head, int y);
segment_t* SEG_get(segment_t *segments, int index);
segment_t* SEG_closest_to_pt(segment_t *seg, int type, int x, int y);
segment_t* SEG_filter_by_rect( segment_t* obstacles, int r_x1, int r_y1, int r_x2, int r_y2);

int SEG_determine_type(segment_t*  seg);
int SEG_len(segment_t *obstacles);
int SEG_common_x(segment_t *first, segment_t *second);
int SEG_common_y(segment_t *first, segment_t *second);

bool SEG_contains(segment_t *seg, int x, int y);

void SEG_push(segment_t** head, int x1, int y1, int x2, int y2);
void SEG_free(segment_t* head);
void SEG_delete(segment_t** head, int y);
void SEG_merge(segment_t** head, segment_t* candidates);
void SEG_debug(segment_t* seg);

#endif
