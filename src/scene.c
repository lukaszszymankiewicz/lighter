#include "data/library.h"

#include <stdlib.h>
#include <stdbool.h>

#include "gl_util.h"
#include "render.h"
#include "scene.h"

scene_t *scene = NULL;

void SCENE_clear(
    scene_t* scene
) {
    scene->n_tile   = 0;
    scene->n_sprite = 0;
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

void SCENE_free(
    scene_t* scene
) {
    free(scene);      
    scene = NULL;
}

void SCENE_draw(
    scene_t* scene 
) {
    for (int tile=0; tile<scene->n_tile; tile++) {
         RENDER_texture(
            tilesets_library[scene->tile_layer[tile].id],
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

    for (int sprite=0; sprite<scene->n_sprite; sprite++) {
        RENDER_texture(
            sprites_library[scene->sprite_layer[sprite].id],
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
}
