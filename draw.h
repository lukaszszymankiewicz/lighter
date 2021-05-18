#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct texture {
    SDL_Texture* surface;
    int width;
    int height;
} Texture;

SDL_Window* create_window(int width, int height);
void init_video();
SDL_Renderer* init_renderer(SDL_Window* window);
void init_png();
int init_graphics(int width, int height);
Texture *init_texture(char *filepath);
void load_frames();
void free_texture(Texture *texture);
void free_graphics();
void render_texture(Texture* texture, SDL_Rect* clip, int x, int y);
void draw_hero_sprite(int x, int y, int hero_state);
void clear_screen();
void update_graphics();
void draw_ray(int x1, int y1, int x2, int y2);
