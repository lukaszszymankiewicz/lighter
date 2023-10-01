#include "library.h"

#include "../gfx.h"

texture_t *tilesets_library[TILESET_ALL] = { NULL };

texture_blueprint_t texture_blueprint_basic = {
    "./src/data/tilesets/basic.png"
};

void LIB_create_tilesets_library(
) {
    tilesets_library[TILESET_BASIC] = GFX_read_texture(texture_blueprint_basic.filepath);
};

