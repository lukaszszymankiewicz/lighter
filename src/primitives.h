#ifndef SEGMENT_H
#define SEGMENT_H

// single linked list of obstacle segments
typedef struct obstacle {
    int x1; int y1;            // first point
    int x2; int y2;            // second point
    float slope;               // slope coeficient
    struct obstacle* next;     // next obstacle
} obstacle_t;

// ray is represented by segment
typedef struct ray {
    int x1; int y1;
    int x2; int y2;
} ray_t;

typedef struct vertex {
    int x; int y;         // coords of vertex
    float angle;          // angle it makes with player
    float slope;          // slope vertex made with next vertex
    struct vertex* next;  // next vertex
    struct vertex* prev;  // prev vertex
} vertex_t;

#endif
