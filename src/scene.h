#include <stdbool.h>

#include <GL/gl.h>

#include "gl_util.h"

#ifndef SCENE_H
#define SCENE_H

#define MAX_LAYERS_ON_SCENE           3
#define MAX_DRAWBLE_OBJECTS_ON_LAYER  10
#define MAX_SHADER_ON_SHADER_LAYER    100

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
    GLenum            mode;
    drawable_shader_t objs[MAX_DRAWBLE_OBJECTS_ON_LAYER];
} layer_t; 

typedef struct scene {
    int                n_layers;
    int                cur_layer;
    layer_t            layers[MAX_LAYERS_ON_SCENE];
} scene_t; 

extern scene_t *scene;

scene_t* SCENE_new(int n_layers);

void SCENE_clear(scene_t* scene);
void SCENE_attach_shader(int layer, int shader_id, GLenum mode);
void SCENE_add_new_drawable_object(scene_t *scene);
void SCENE_set_texture(scene_t *scene, float *texture);
void SCENE_add_uniform(scene_t *scene, float *value);
void SCENE_add_vertices(scene_t *scene, int len, float *vertices, int count);
void SCENE_activate_layer(scene_t *scene, int layer);
void SCENE_free(scene_t* scene);
void SCENE_draw(scene_t* scene);

#endif
