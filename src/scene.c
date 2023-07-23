#include <GL/glew.h>
#include <GL/gl.h>

#include "data/library.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gl_util.h"
#include "render.h"
#include "scene.h"

static const int TILE_LAYER_PROGRAM_ID   = 0;
static const int SPRITE_LAYER_PROGRAM_ID = 0;

scene_t *scene = NULL;

void SCENE_clear(
    scene_t* scene
) {
    scene->n_tile   = 0;
    scene->n_sprite = 0;
    scene->n_shader = 0;
}

scene_t* SCENE_new(
) {
    scene_t *new_scene   = NULL;
    new_scene            = (scene_t*)malloc(sizeof(scene_t));

    SCENE_clear(new_scene);

    return new_scene;
}

void SCENE_add_tile(
    scene_t* scene,
    int            id,
    render_coord_t render,
    render_coord_t clip
) {
    if (scene->n_tile >= MAX_TILES_ON_TILE_LAYER) { return; }

    scene->tile_layer[scene->n_tile].id     = id;
    scene->tile_layer[scene->n_tile].render = render;
    scene->tile_layer[scene->n_tile].clip   = clip;

    scene->n_tile++;
}

void SCENE_add_sprite(
    scene_t* scene,
    int            id,
    render_coord_t render,
    render_coord_t clip,
    bool           flip_w,
    bool           flip_h
) {
    if (scene->n_sprite >= MAX_SPRITES_ON_SPRITES_LAYER) { return; }

    scene->sprite_layer[scene->n_sprite].id     = id;
    scene->sprite_layer[scene->n_sprite].render = render;
    scene->sprite_layer[scene->n_sprite].clip   = clip;
    scene->sprite_layer[scene->n_sprite].flip_w = flip_w;
    scene->sprite_layer[scene->n_sprite].flip_h = flip_h;

    scene->n_sprite++;
}

void SCENE_add_shader(
    scene_t *scene,
    int      id,
    int      program_id,
    int      len,
    int      size,
    GLfloat *vertices
) {
    if (scene->n_shader >= MAX_SHADER_ON_SHADER_LAYER) { return; }

    scene->shader_layer[scene->n_shader].id         = id;
    scene->shader_layer[scene->n_shader].program_id = program_id;
    scene->shader_layer[scene->n_shader].len        = len;
    scene->shader_layer[scene->n_shader].size       = size;

    for (int i=0; i<len; i++) {
        scene->shader_layer[scene->n_shader].vertices[i] = vertices[i];
    }

    scene->n_shader++;
}

void SCENE_free(
    scene_t* scene
) {
    free(scene);      
    scene = NULL;
}

void SCENE_draw(
    scene_t* scene 
) {
    glUseProgram(TILE_LAYER_PROGRAM_ID);
    for (int tile=0; tile<scene->n_tile; tile++) {
         RENDER_texture(
            tilesets_library[scene->tile_layer[tile].id]->id,
            scene->tile_layer[tile].render.x1,
            scene->tile_layer[tile].render.y1,
            scene->tile_layer[tile].render.x2,
            scene->tile_layer[tile].render.y2,
            scene->tile_layer[tile].clip.x1,
            scene->tile_layer[tile].clip.y1,
            scene->tile_layer[tile].clip.x2,
            scene->tile_layer[tile].clip.y2,
            false
        );
    }

    glUseProgram(SPRITE_LAYER_PROGRAM_ID);
    for (int sprite=0; sprite<scene->n_sprite; sprite++) {
        RENDER_texture(
            sprites_library[scene->sprite_layer[sprite].id]->id,
            scene->sprite_layer[sprite].render.x1,
            scene->sprite_layer[sprite].render.y1,
            scene->sprite_layer[sprite].render.x2,
            scene->sprite_layer[sprite].render.y2,
            scene->sprite_layer[sprite].clip.x1,
            scene->sprite_layer[sprite].clip.y1,
            scene->sprite_layer[sprite].clip.x2,
            scene->sprite_layer[sprite].clip.y2,
            scene->sprite_layer[sprite].flip_w
        );
    }
    printf("there is %d shaders to render! \n", scene->n_shader);
    for (int shader=0; shader<scene->n_shader; shader++) {
    // for (int shader=0; shader<1; shader++) {
        RENDER_shader(
            scene->shader_layer[shader].vertices,
            scene->shader_layer[shader].program_id,
            scene->shader_layer[shader].len,
            scene->shader_layer[shader].size
        );

    }
}
