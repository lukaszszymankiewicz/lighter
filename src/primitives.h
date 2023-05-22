#include "global.h"

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

typedef short int direction_t;
// TODO: clean it up, please
enum direction { RIGHT, LEFT, UP, DOWN, NONE };
enum segtype { HOR, VER, UNKNOWN };
enum state { STANDING, WALKING, JUMPING, FALLING_DOWN, NOTHING, MAX_STATE };

typedef struct segment {
    int   x1;
    int   y1;
    int   x2; 
    int   y2;
    float slope;
    int   type;
    struct segment *next;
} segment_t;

typedef struct point {
    int           x;
    int           y;
    struct point *next;
} point_t;

typedef struct vertex {
    int            x;
    int            y;
    float          angle;
    struct vertex *next;
    float          dist;
} vertex_t;

typedef struct lvertex {
    vertex_t* coords;
    int       x0;
    int       y0;
    int       power;
    int       red;
    int       green;
    int       blue;
} lvertex_t;

typedef struct light_scene {
    lvertex_t   *components[MAX_LIGHT_ON_SCENE];
    int          n;
} light_scene_t;

#endif
