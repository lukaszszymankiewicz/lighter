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

extern scene_t *scene;

void SCENE_new();
void SCENE_clear();
void SCENE_add_layer(int layer, int shader_id, int mode);
void SCENE_add_buffer_layer(int layer, int shader_id, int mode);
void SCENE_add_new_drawable_object();
void SCENE_set_texture(array_t arr);
void SCENE_add_uniform(array_t arr);
void SCENE_add_vertices(int len, float *vertices, int val_per_vertex);
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

int SCENE_get_layer_buffer_tex(int layer);

#endif
