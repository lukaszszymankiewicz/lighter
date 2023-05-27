#include <stdbool.h>

#ifndef IMG_H
#define IMG_H

#define MAX_TILES_ON_TILE_LAYER 100
#define MAX_SPRITES_ON_SPRITES_LAYER 10

typedef struct render_coord {
    float        x1;       // glVertex2f for x1 (or glTexCoord2f)
    float        y1;       // glVertex2f for y1 (or glTexCoord2f)
    float        x2;       // glVertex2f for x2 (or glTexCoord2f)
    float        y2;       // glVertex2f for y2 (or glTexCoord2f)
} render_coord_t;

typedef struct drawable {
    int            id;
    render_coord_t render;
    render_coord_t clip;
    bool           flip_h;
    bool           flip_w;
} drawable_t;

typedef struct scene {
    int        n_tile;
    int        n_sprite;
    drawable_t tile_layer[MAX_TILES_ON_TILE_LAYER];  
    drawable_t sprite_layer[MAX_SPRITES_ON_SPRITES_LAYER];  
} scene_t; 

scene_t* IMG_new_scene();

void IMG_clear_scene(scene_t* scene);
void IMG_add_tile_to_scene(scene_t* scene, int id, render_coord_t render, render_coord_t clip);
void IMG_add_sprite_to_scene(
    scene_t* scene,
    int id,
    render_coord_t render,
    render_coord_t clip,
    bool flip_w,
    bool flip_h
);
void IMG_free_scene(scene_t* scene);

extern scene_t *scene;

#endif
