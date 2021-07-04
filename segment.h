#ifndef SEGMENT_H
#define SEGMENT_H

typedef struct point {
    short int x;
    short int y;
} point_t;

typedef struct segment {
    point_t beg;
    point_t end;
    struct segment* next;
} segment_t;

void SEG_push(segment_t** head, short int x1, short int y1, short int x2, short int y2);
void SEG_free(segment_t * segments);
bool SEG_intersects(segment_t first, segment_t second, point_t * intersection);
segment_t * SEG_init(short int x1, short int y1, short int x2, short int y2);
point_t * PT_init(short int x, short int y);

#endif
