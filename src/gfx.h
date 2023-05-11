#include "assets.h"
#include "global.h"
#include "primitives.h"
#include "sorted_list.h"
#include "texture.h"

#ifndef GFX_H
#define GFX_H

extern SDL_Window *window;
extern SDL_Surface *surface;
extern SDL_Renderer *renderer;

extern float global_x_scale;
extern float global_y_scale;

typedef void (*shader_t)(uint32_t, int, int, int, int, int);

int GFX_init_graphics();

void GFX_free();
void GFX_clear_screen();
void GFX_update();

void GFX_draw_colored_line(int x1, int y1, int x2, int y2, int r, int g, int b, int a);
void GFX_fill_lightbuffer(uint32_t color, int x, int y, int power, int x0, int y0);

void GFX_fill_light(
    void (*pix_fill_fun)(uint32_t, int, int, int, int, int),
    vertex_t *poly,
    int r,
    int g,
    int b,
    int power,
    int x0,
    int y0
);

void GFX_fill_rect(
    shader_t shader,
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
void GFX_draw_light();
void GFX_draw_darkness();

void GFX_render_texture_part(
    texture_t        *texture,
    float render_x1, float render_y1,
    float render_x2, float render_y2,
    float tex_x1,    float tex_y1,
    float tex_x2,    float tex_y2,
    bool flip
);

#endif
