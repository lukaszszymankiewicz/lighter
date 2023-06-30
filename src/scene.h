#include <stdbool.h>

#include "gl_util.h"

#ifndef SCENE_H
#define SCENE_H

#define MAX_TILES_ON_TILE_LAYER      100
#define MAX_SPRITES_ON_SPRITES_LAYER 10
#define MAX_VERTEX_ON_SHADER_LAYER   1

typedef struct drawable_texture {
    int            id;
    render_coord_t render;
    render_coord_t clip;
    bool           flip_h;
    bool           flip_w;
} drawable_texture_t;

typedef struct drawable_shader {
    int            id;
    int            len;
    int            program_id;
    int            size;
    GLfloat       *vertices;
} drawable_shader_t;

typedef struct scene {
    int                n_tile;
    int                n_sprite;
    int                n_shader;
    drawable_texture_t tile_layer[MAX_TILES_ON_TILE_LAYER];  
    drawable_texture_t sprite_layer[MAX_SPRITES_ON_SPRITES_LAYER];  
    drawable_shader_t  shader_layer[MAX_VERTEX_ON_SHADER_LAYER];  
} scene_t; 

extern scene_t *scene;

scene_t* SCENE_new();

void SCENE_clear(scene_t* scene);
void SCENE_add_tile(
    scene_t* scene,
    int id,
    render_coord_t render,
    render_coord_t clip
);
void SCENE_add_sprite(
    scene_t* scene,
    int            id,
    render_coord_t render,
    render_coord_t clip,
    bool           flip_w,
    bool           flip_h
);
void SCENE_add_shader(
    scene_t *scene,
    int      id,
    int      program_id,
    int      len,
    int      size,
    GLfloat *vertices
);

void SCENE_free(scene_t* scene);
void SCENE_draw(scene_t* scene);

#endif
