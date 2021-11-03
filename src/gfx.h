#include "def.h"
#include "primitives.h"
#include "texture.h"

#ifndef GFX_H
#define GFX_H


extern SDL_Window *window;
extern SDL_Surface *surface;
extern SDL_Renderer *renderer;

SDL_Window *GFX_init_window(int w, int h);
SDL_Renderer *GFX_init_renderer(SDL_Window *window);

int GFX_init_graphics(int width, int height);

void GFX_free();
void GFX_clear_screen();
void GFX_update();

texture_t* GFX_read_texture(char *filepath);

void GFX_draw_colored_line(float x1, float y1, float x2, float y2, int r, int g, int b, int a);
void GFX_draw_light_polygon(vertex_t *poly, int r, int g, int b, int a);
void GFX_render_texture(texture_t* texture, SDL_Rect* clip, int x, int y, bool flip);
void GFX_render_tile(texture_t* texture, int render_x, int render_y, int x, int y, int w, int h);
void GFX_free_texture(texture_t *texture);

void GFX_debug_obs(obstacle_t* obstacles);

#endif
