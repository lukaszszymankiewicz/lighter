#include "global.h"

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

typedef short int direction_t;
enum direction { RIGHT, LEFT, UP, DOWN, NONE };

// for calculating hits
typedef struct obstacle {
    int              x1;
    int              y1;
    int              x2;
    int              y2;
    struct obstacle *next;
} obstacle_t;

// for drawing
typedef struct segment {
    int   x1;
    int   y1;
    int   x2; 
    int   y2;
    float slope;
    struct segment *next;
} segment_t;

typedef struct point {
    int           x;
    int           y;
    struct point *next;
} point_t;

// light vertex representation
typedef struct vertex {
    int            x;         // coords of vertex
    int            y;         // coords of vertex
    float          angle;     // angle it makes with player
    struct vertex *next;      // next vertex
    struct vertex *prev;      // prev vertex
} vertex_t;

typedef struct texture {
    SDL_Texture *surface;
    int          width;
    int          height;
} texture_t;

#endif
