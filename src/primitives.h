#ifndef PRIMITIVES_H
#define PRIMITIVES_H

typedef short int direction_t;
enum direction { RIGHT, LEFT, UP, DOWN, NONE };
enum texture_data { TILESET_INDEX, X_OFFSET, Y_OFFSET };

typedef struct texture {
    SDL_Texture *surface;
    int          width;
    int          height;
} texture_t;

// single linked list of obstacle segments
typedef struct obstacle {
    int              x1;
    int              y1;
    int              x2;
    int              y2;
    float            slope;
    struct obstacle *next;     // next obstacle
} obstacle_t;

typedef struct ray {
    int x1;
    int y1;
    int x2; 
    int y2;
} ray_t;

// light vertex representation
typedef struct vertex {
    int            x;         // coords of vertex
    int            y;         // coords of vertex
    float          angle;     // angle it makes with player
    float          slope;     // slope vertex made with next vertex
    struct vertex *next;      // next vertex
    struct vertex *prev;      // prev vertex
} vertex_t;

#endif
