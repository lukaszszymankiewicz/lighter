#include <stdbool.h>

#include <GL/gl.h>

#include "gl_util.h"

enum LAYER_TYPE {
    LAYER_TYPE_TEXTURE,
    LAYER_TYPE_SHADER,
    LAYER_TYPE_ALL
};

#ifndef SCENE_H
#define SCENE_H

#define MAX_LAYERS_ON_SCENE           3
#define MAX_DRAWBLE_OBJECTS_ON_LAYER  100
#define MAX_TILES_ON_TILE_LAYER       100
#define MAX_VERTEX_ON_TILE_LAYER      100
#define MAX_VERTEX_ON_SHADER_LAYER    50
#define MAX_SPRITES_ON_SPRITES_LAYER  10
#define MAX_SHADER_ON_SHADER_LAYER    10
#define MAX_SHADER_INT_ARGS           3

typedef struct drawable_texture {
    int            texture_id;
    render_coord_t render;
    render_coord_t clip;
    bool           flip_h;
    bool           flip_w;
} drawable_texture_t;

typedef struct drawable_shader {
    int         program_id;
    int         len;
    GLfloat     vertices[MAX_VERTEX_ON_SHADER_LAYER];    
    float       uniforms[MAX_SHADER_UNIFORMS_ARGS];
} drawable_shader_t;

union drawable{
    drawable_texture_t texture;
    drawable_shader_t  shader;
};

typedef struct layer {
    bool   on;
    int    type;
    int    n_objs;
    int    program_id;
    union  drawable objs[MAX_DRAWBLE_OBJECTS_ON_LAYER];
} layer_t; 

typedef struct scene {
    int                n_layers;
    layer_t            layers[MAX_LAYERS_ON_SCENE];
} scene_t; 

extern scene_t *scene;

scene_t* SCENE_new(
    int n_tile_layers,
    int n_shader_layers
);

void SCENE_clear(scene_t* scene);

void SCENE_add_texture(
    scene_t       *scene,
    int            layer,
    int            texture_id,
    render_coord_t render,
    render_coord_t clip,
    bool           flip_w,
    bool           flip_h
);

void SCENE_add_shader(
    scene_t    *scene,
    int         layer,
    int         program_id,
    int         len,
    GLfloat    *vertices,
    float      *uniforms
);

void SCENE_free(scene_t* scene);
void SCENE_draw(scene_t* scene);

#endif
