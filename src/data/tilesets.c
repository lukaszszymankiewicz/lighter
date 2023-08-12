#include "library.h"
#include "util.h"

#include "../texture.h"

texture_t *tilesets_library[TILESET_ALL] = { NULL };

texture_blueprint_t texture_blueprint_basic = {
    "./src/data/tilesets/basic.png"
    // "./src/data/tilesets/dog.png"
};

void LIB_create_tilesets_library(
) {
    tilesets_library[TILESET_BASIC] = TXTR_read_from_file(texture_blueprint_basic.filepath);
};

