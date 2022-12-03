#include "assets.h"
#include "global.h"
#include "primitives.h"
#include "sorted_list.h"

#ifndef GFX_H
#define GFX_H

extern SDL_Window *window;
extern SDL_Surface *surface;
extern SDL_Renderer *renderer;
extern texture_t         *gradients[ASSET_GRADIENT_ALL];
extern texture_t         *sprites[ASSET_SPRITE_ALL];

void GFX_init_window();
void GFX_init_renderer();

int GFX_init_graphics();

void GFX_free();
void GFX_clear_screen();
void GFX_update();

void GFX_draw_colored_line(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
void GFX_fill_shadowbuffer(uint32_t color, int x, int y, int power);
void GFX_fill_lightbuffer(uint32_t color, int x, int y, int power);
void GFX_fill_mesh_shadowbuffer(uint32_t color, int x, int y, int power);

void GFX_fill_buffer_single_polygon(
    vertex_t *poly,
    void (*pix_fill_fun)(uint32_t, int, int, int),
    int r,
    int g,
    int b,
    int power
);

void GFX_fill_rect(
    void (*pix_fill_fun)(uint32_t, int, int, int),
    int x,
    int y,
    int w,
    int h,
    int r,
    int g,
    int b,
    int power 
);

void GFX_draw_rect_border(int x, int y, int w, int h, int r, int g, int b, int a);
void GFX_render_texture(texture_t* texture, SDL_Rect* clip, int x, int y, bool flip);
void GFX_render_tile(texture_t* texture, int render_x, int render_y, int x, int y, int w, int h);
void GFX_free_texture(texture_t *texture);
void GFX_clean_buffers();
void GFX_draw_lightbuffer(texture_t* gradient, int x, int y);

texture_t* GFX_read_texture(const char *filepath);

// for tests
sorted_list_t* GFX_calc_intersections_in_scanline(segment_t *segments, int y, int *n);

#endif
