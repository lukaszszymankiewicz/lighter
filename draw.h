#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

typedef struct lightpoint {
    int x;
    int y;
    int wall_id;
    float angle;
    struct lightpoint* next;
} lightpoint_t;

typedef struct polyline {
    int x1;
    int y1;
    int x2;
    int y2;
    struct polyline * next;
} polyline_t;

typedef struct texture {
    SDL_Texture* surface;
    int width;
    int height;
} Texture;

typedef struct position {
    int x; 
    int y; 
} position_t;

// INIT
SDL_Window* create_window(int width, int height);
SDL_Renderer* init_renderer(SDL_Window* window);
int init_graphics(int width, int height);
void load_frames();
void init_video();
void init_png();
void enable_alpha_bleeding();
Texture *init_texture(char *filepath);

// FREE
void LRE_free_graphics();
void LRE_clear_screen();

// UPDATE
void LRE_update_graphics();

// TEXTURE
void render_texture(Texture* texture, SDL_Rect* clip, int x, int y);

// DRAW
void LRE_draw_hero_sprite(int x, int y, int hero_state);
void LRE_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a);
void LRE_draw_rays(position_t hero_pos, polyline_t * walls); 

// STRUCTURES
polyline_t * init_polyline(int x1, int y1, int x2, int y2);
void insert_lightpoint(lightpoint_t** head, int x1, int y1, int x2, int y2, int wall_id);
void insert_polyline(polyline_t** head, polyline_t* new_polyline);
void free_polyline(lightpoint_t* head);
