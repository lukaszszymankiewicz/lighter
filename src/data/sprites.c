#include "library.h"
#include "util.h"

#include "../texture.h"

texture_t *sprites_library[SPRITE_ALL] = {NULL};

texture_blueprint_t texture_blueprint_hero = {
    "./src/data/sprites/hero.png"
};

texture_blueprint_t texture_blueprint_lighter = {
    "./src/data/sprites/lighter.png"
};

texture_blueprint_t texture_blueprint_walllight = {
    "./src/data/sprites/walllight.png"
};

void LIB_create_sprites_library(
) {
    sprites_library[SPRITE_HERO]       = TXTR_read_from_file(texture_blueprint_hero.filepath);
    sprites_library[SPRITE_LIGHTER]    = TXTR_read_from_file(texture_blueprint_lighter.filepath);
    sprites_library[SPRITE_WALLLIGHT]  = TXTR_read_from_file(texture_blueprint_walllight.filepath);
};
