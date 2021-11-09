#include "def.h"
#include "primitives.h"
#include "texture.h"
#include "intersection.h"

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

void GFX_draw_colored_line(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
void GFX_draw_light_polygon(vertex_t *poly, int r, int g, int b, int a);
void GFX_render_texture(texture_t* texture, SDL_Rect* clip, int x, int y, bool flip);
void GFX_render_tile(texture_t* texture, int render_x, int render_y, int x, int y, int w, int h);
void GFX_free_texture(texture_t *texture);

void GFX_debug_obs(obstacle_t* obstacles);

// for tests
x_intersection_t* GFX_calc_intersections_in_scanline(segment_t *segments, int y, int *n);

#endif
