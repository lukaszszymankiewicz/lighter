#include "global.h"
#include "tile.h"
#include "level.h"
#include "gfx.h"
#include "sprites.h"
#include "light.h"

#ifndef IMPORT_H
#define IMPORT_H

enum READ_LEVEL {
    READ_LEVEL_IDLE,
    READ_LEVEL_X_SIZE,
    READ_LEVEL_Y_SIZE, 
    READ_LEVEL_TILES_SUM,
    READ_LEVEL_TILE_SUM,
    READ_LEVEL_TILE_X,
    READ_LEVEL_TILE_Y,
    READ_LEVEL_ALL_TILES_READ,
};

enum READ_ANIMATION {
    READ_ANIMATION_PREAMBULE_IDLE,
    READ_ANIMATION_PREAMBULE_FIRST_HALF,
    READ_ANIMATION_PREAMBULE_SECOND_HALF,
    READ_ANIMATIONS_NUMBER,
    READ_ANIMATION_IDX,
    READ_ANIMATION_N_FRAMES,
    READ_ANIMATION_DELAY,
    READ_ANIMATION_RECT,
    READ_ANIMATION_HITBOX_PER_FRAME,
    READ_ANIMATION_HITBOX_RECT,
    READ_ANIMATION_ALL_READ,
};

enum READ_WOBBLE {
    READ_WOBBLE_IDLE,
    READ_WOBBLE_PREAMBULE_FIRST_HALF,
    READ_WOBBLE_PREAMBULE_SECOND_HALF,
    READ_WOBBLE_NUMBER,
    READ_WOBBLE_SIGN, 
    READ_WOBBLE_COEF, 
    READ_WOBBLE_ALL_READ,
};

enum ASSETS_ANIMATION_IDX {
    ASSET_HERO_ANIMATION,
    ASSET_ANIMATION_ALL
};

enum ASSETS_GRADIENT_IDX {
    ASSET_GRADIENT_CIRCULAR,
    ASSET_GRADIENT_ALL
};

enum ASSETS_SPRITE_IDX {
    ASSET_SPRITE_HERO,
    ASSET_SPRITE_ALL
};

enum ASSETS_WOBBLE_IDX {
    ASSET_WOBBLE_NO,
    ASSET_WOBBLE_STABLE,
    ASSET_WOBBLE_WALKING,
    ASSET_WOBBLE_ALL
};

enum ASSETS_LEVEL_IDX {
    ASSET_LEVEL_SAMPLE,
    ASSET_LEVEL_ALL
};

extern animation_sheet_t *animations[ASSET_ANIMATION_ALL];
extern texture_t         *gradients[ASSET_GRADIENT_ALL];
extern texture_t         *sprites[ASSET_SPRITE_ALL];
extern wobble_t          *wobbles[ASSET_WOBBLE_ALL];
extern level_t           *levels[ASSET_LEVEL_ALL];

level_t*           IMP_read_level(const char* filepath);
animation_sheet_t* IMP_read_animation(const char* filepath);
texture_t*         IMP_read_texture(const char* filepath);
wobble_t*          IMP_read_wobble(const char* filepath);

void               IMP_read_all_files();

#endif
