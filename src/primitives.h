#ifndef PRIMITIVES_H
#define PRIMITIVES_H

typedef short int direction_t;
enum direction { RIGHT, LEFT, UP, DOWN, NONE };

// single linked list of obstacle segments
typedef struct obstacle {
    int x1; int y1;            // first point
    int x2; int y2;            // second point
    float slope;               // slope coeficient
    struct obstacle* next;     // next obstacle
} obstacle_t;

// light ray representation
typedef struct ray {
    int x1; int y1;            // start point coords
    int x2; int y2;            // end point coords
} ray_t;

// light vertex representation
typedef struct vertex {
    int x; int y;         // coords of vertex
    float angle;          // angle it makes with player
    float slope;          // slope vertex made with next vertex
    struct vertex* next;  // next vertex
    struct vertex* prev;  // prev vertex
} vertex_t;


#endif
