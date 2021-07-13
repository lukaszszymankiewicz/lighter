#ifndef SEGMENT_H
#define SEGMENT_H

typedef struct point {
    int x;
    int y;
} point_t;

typedef struct segment {
    point_t beg;
    point_t end;
    struct segment* next;
} segment_t;

segment_t * SEG_init(int x1, int y1, int x2, int y2);
point_t * PT_init(int x, int y);
void SEG_push(segment_t** head, int x1, int y1, int x2, int y2);
void SEG_free(segment_t * segments);
int SEG_intersects (segment_t first, segment_t second, point_t * intersection);

#endif
