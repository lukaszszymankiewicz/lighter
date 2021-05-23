#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct texture {
    SDL_Texture* surface;
    int width;
    int height;
} Texture;

typedef struct intersection_point {
    float x;
    float y;
    float T1;
    bool found;
} IntersectionPoint;

typedef struct point {
    float x; 
    float y; 
} Point;

typedef struct line {
    float x1;
    float y1;
    float x2;
    float y2; 
} Line;

typedef struct polygon {
    Line lines[10];
    int len;
} Polygon;

// INIT
int init_graphics(int width, int height);
void load_frames();

// FREE
void free_graphics();
void clear_screen();

// UPDATE
void update_graphics();

// DRAW
void draw_hero_sprite(int x, int y, int hero_state);
void draw_circle(int center_x, int center_y, int r, int power);
void draw_wall(int x0, int y0, int x1, int y1);
void draw_walls(Polygon *obstacles);
void draw_rays(float st_x, float st_y, Polygon *obstacles);
