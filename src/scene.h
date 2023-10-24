#include <stdbool.h>

#include "gl_util.h"
#include "mat.h"

#ifndef SCENE_H
#define SCENE_H

#define MAX_LAYERS_ON_SCENE           4
#define MAX_DRAWBLE_OBJECTS_ON_LAYER  10
#define DEFAULT_FRAMEBUFFER           0
#define NO_TEXTURE                    0

typedef struct drawable_shader {
    int         texture;
    int         len;
    array_t     vertices;    
    float      *uniforms[MAX_SHADER_UNIFORMS];
    int         uniform_count;
    int         count;
} drawable_shader_t;

typedef struct layer {
    bool              on;
    int               n_objs;
    int               shader_id;
    framebuffer_t    *framebuffer;
    int               mode;
    drawable_shader_t objs[MAX_DRAWBLE_OBJECTS_ON_LAYER];
} layer_t; 

typedef struct scene {
    int                n_layers;
    int                cur_layer;
    layer_t            layers[MAX_LAYERS_ON_SCENE];
} scene_t; 

void SCENE_init();
void SCENE_clear();
void SCENE_add_layer(int layer, int shader_id, int mode);
void SCENE_add_buffer_layer(int layer, int shader_id, int mode);
void SCENE_activate_layer(int layer);
void SCENE_free();
void SCENE_draw();

void SCENE_draw_texture(
    int   draw_x, int   draw_y,
    int   clip_x, int   clip_y,
    int        w, int        h,
    int    tex_w, int    tex_h,
    bool  flip_w, bool  flip_h,
    int  texture
);

void SCENE_draw_polygon(
    float *vertices,
    int   len,
    float r,
    float g,
    float b,
    float a
);

int SCENE_get_layer_buffer_tex(int layer);

#endif
