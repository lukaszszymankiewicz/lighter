#include <stdbool.h>

#include <GL/gl.h>

#include "gl_util.h"

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

typedef struct drawable_shader {
    int         program_id;
    int         texture;
    int         len;
    GLfloat     vertices[MAX_VERTEX_ON_SHADER_LAYER];    
    float       uniforms[MAX_SHADER_UNIFORMS_ARGS];
} drawable_shader_t;

typedef struct layer {
    bool              on;
    int               n_objs;
    int               program_id;
    drawable_shader_t objs[MAX_DRAWBLE_OBJECTS_ON_LAYER];
} layer_t; 

typedef struct scene {
    int                n_layers;
    layer_t            layers[MAX_LAYERS_ON_SCENE];
} scene_t; 

extern scene_t *scene;

scene_t* SCENE_new(n_layers);

void SCENE_clear(scene_t* scene);

void SCENE_add_shader(
    scene_t    *scene,
    int         layer,
    int         texture,
    int         shader_program_id,
    int         len,
    GLfloat    *vertices,
    float      *uniforms
);

void SCENE_free(scene_t* scene);
void SCENE_draw(scene_t* scene);

#endif
