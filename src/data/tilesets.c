#include "library.h"

#include "../gfx.h"

texture_t *tilesets_library[TILESET_ALL] = { NULL };

texture_blueprint_t texture_blueprint_basic = {
    TILESET_BASIC,
    "./src/data/tilesets/basic.png"
};

void LIB_read_tileset_blueprint(
    texture_blueprint_t *blueprint
) {
    texture_t* tileset = GFX_read_texture(blueprint->filepath);

    if (tileset) {
        printf("TILESET BLUEPRINT ID = %d read\n", blueprint->id);
    }

    tilesets_library[blueprint->id]      = tileset;
};

void LIB_create_tilesets_library(
) {
    LIB_read_tileset_blueprint(&texture_blueprint_basic);
};

