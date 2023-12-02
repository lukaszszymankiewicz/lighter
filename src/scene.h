#include <stdbool.h>

#include "mat.h"

#ifndef SCENE_H
#define SCENE_H

#define MAX_LAYERS_ON_SCENE           5
#define MAX_BUFFERS_ON_SCENE          3
#define MAX_DRAWBLE_OBJECTS_ON_LAYER  10
#define NO_TEXTURE                    0

typedef struct  shader_params {
    int         shader_id;
    int         texture;
    int         len;
    array_t     vertices;    
    float      *uniforms[MAX_SHADER_UNIFORMS];
    int         uniform_count;
    int         count;
    int         mode;
} shader_params_t;

typedef struct layer {
    bool              on;
    int               n_objs;
    shader_params_t   objs[MAX_DRAWBLE_OBJECTS_ON_LAYER];
} layer_t; 

typedef struct scene {
    int                n_layers;
    int                n_buffers;
    int                cur_layer;
    int                cur_buffer;
    layer_t            layers[MAX_LAYERS_ON_SCENE];
    framebuffer_t     *buffers[MAX_BUFFERS_ON_SCENE];    
} scene_t; 

int SCENE_get_buffer_tex();
int SCENE_get_buffer_width();
int SCENE_get_buffer_height();

void SCENE_init();
void SCENE_clear();
void SCENE_add_layer(int layer);
void SCENE_activate_buffer(int buffer);
void SCENE_activate_layer(int layer);
void SCENE_add_defalt_buffer();
void SCENE_add_buffer(int id, int w, int h);

void SCENE_draw_scaled_buffer();

void SCENE_clean_buffer(int buffer);
void SCENE_free();
void SCENE_render_current_layer();
void SCENE_clear_layer(int layer);
void SCENE_add_new_drawable_object();

void SCENE_put_texture_to_scene(
    int   draw_x, int   draw_y,
    int   draw_h, int   draw_w,
    int   clip_x, int   clip_y,
    int   clip_w, int   clip_h,
    int    tex_w, int    tex_h,
    bool  flip_w, bool  flip_h,
    int  texture
);

void SCENE_put_polygon_to_scene(
    float *vertices,
    int    n_coords,
    int    x0,
    int    y0,
    int    r,
    int    g,
    int    b,
    int    a
);

#endif
