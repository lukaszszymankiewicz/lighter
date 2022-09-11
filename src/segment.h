#include "primitives.h"

#ifndef SEGMENT_H
#define SEGMENT_H

segment_t* SEG_init(int x1, int y1, int x2, int y2);
segment_t* SEG_get_segments_of_polygon(vertex_t *poly);
segment_t* SEG_find_candidates(segment_t** head, int y);
int SEG_determine_type(segment_t*  seg);
void SEG_push(segment_t** head, int x1, int y1, int x2, int y2);
void SEG_free(segment_t* obstacles);
void SEG_delete(segment_t** head, int y);
void SEG_merge(segment_t** head, segment_t* candidates);
int SEG_len(segment_t *obstacles);
segment_t* SEG_get(segment_t *segments, int index);
int SEG_common_x(segment_t *first, segment_t *second);
int SEG_common_y(segment_t *first, segment_t *second);
bool SEG_contains(segment_t *seg, int x, int y);
segment_t* SEG_closest_to_pt(segment_t *seg, int type, int x, int y);

#endif
