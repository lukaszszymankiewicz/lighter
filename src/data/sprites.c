#include "library.h"

#include "../gfx.h"

texture_t *sprites_library[SPRITE_ALL] = { NULL };

texture_blueprint_t texture_blueprint_hero = {
    SPRITE_HERO,
    "./src/data/sprites/hero.png"
};

texture_blueprint_t texture_blueprint_lighter = {
    SPRITE_LIGHTER,
    "./src/data/sprites/lighter.png"
};

texture_blueprint_t texture_blueprint_walllight = {
    SPRITE_WALLLIGHT,
    "./src/data/sprites/walllight.png"
};

texture_blueprint_t texture_blueprint_lamp_small = {
    SPRITE_LAMP_SMALL,
    "./src/data/sprites/lamp_small.png"
};

texture_blueprint_t texture_blueprint_lamp_big = {
    SPRITE_LAMP_BIG,
    "./src/data/sprites/lamp_big.png"
};

void LIB_read_sprites_blueprint(
    texture_blueprint_t *blueprint
) {
    texture_t* sprite = GFX_read_texture(blueprint->filepath);

    if (sprite) {
        printf("SPRITE BLUEPRINT ID = %d read\n", blueprint->id);
    }
    sprites_library[blueprint->id]      = sprite;
};

void LIB_create_sprites_library(
) {
    LIB_read_sprites_blueprint(&texture_blueprint_hero);
    LIB_read_sprites_blueprint(&texture_blueprint_lighter);
    LIB_read_sprites_blueprint(&texture_blueprint_walllight);
    LIB_read_sprites_blueprint(&texture_blueprint_lamp_small);
    LIB_read_sprites_blueprint(&texture_blueprint_lamp_big);
};
