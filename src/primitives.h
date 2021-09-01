#ifndef SEGMENT_H
#define SEGMENT_H

// single linked list of obstacle segments
typedef struct obstacle {
    int x1;
    int y1;
    int x2;
    int y2;
    struct obstacle* next;
} obstacle_t;

// ray is represented by segment
typedef struct ray {
    int x1; 
    int y1;
    int x2;
    int y2;
} ray_t;

#endif
