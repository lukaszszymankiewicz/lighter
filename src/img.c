#include <stdlib.h>
#include <stdbool.h>

#include "img.h"

scene_t *scene = NULL;

void IMG_clear_scene(
    scene_t* scene
) {
    scene->n_tile   = 0;
    scene->n_sprite = 0;
}

scene_t* IMG_new_scene(
) {
    scene_t *new_scene   = NULL;
    new_scene            = (scene_t*)malloc(sizeof(scene_t));

    IMG_clear_scene(new_scene);

    return new_scene;
}

void IMG_add_tile_to_scene(
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

void IMG_add_sprite_to_scene(
    scene_t* scene,
    int            id,
    render_coord_t render,
    render_coord_t clip,
    bool           flip_w,
    bool           flip_h
) {
    if (scene->n_tile >= MAX_SPRITES_ON_SPRITES_LAYER) { return; }

    scene->tile_layer[scene->n_sprite].id     = id;
    scene->tile_layer[scene->n_sprite].render = render;
    scene->tile_layer[scene->n_sprite].clip   = clip;
    scene->tile_layer[scene->n_sprite].flip_w = flip_w;
    scene->tile_layer[scene->n_sprite].flip_h = flip_h;

    scene->n_sprite++;
}

void IMG_free_scene(
    scene_t* scene
) {
    free(scene);      
    scene = NULL;
}
