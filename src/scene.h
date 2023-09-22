#include <stdbool.h>

#include "gl_util.h"

#ifndef SCENE_H
#define SCENE_H

#define MAX_LAYERS_ON_SCENE           3
#define MAX_DRAWBLE_OBJECTS_ON_LAYER  10

typedef struct drawable_shader {
    int         texture;
    int         len;
    float      *vertices;    
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
void SCENE_attach_shader(int layer, int shader_id, int mode);
void SCENE_add_new_drawable_object();
void SCENE_set_texture(float *texture);
void SCENE_add_uniform(float *value);
void SCENE_add_vertices(int len, float *vertices, int val_per_vertex);
void SCENE_activate_layer(int layer);
void SCENE_free();
void SCENE_draw();

#endif
