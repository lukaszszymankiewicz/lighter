#include "library.h"

void LIB_create_all(
) {
    LIB_create_entity_library();
    LIB_create_wobble_library();
    LIB_create_lightsources_library();
    LIB_create_sprites_library();
    LIB_create_levels_library();
    LIB_create_tile_library();
    LIB_create_tilesets_library();
}

void LIB_free_all(
) {
    TXTR_free(sprites_library[SPRITE_HERO]);
    TXTR_free(sprites_library[SPRITE_LIGHTER]);
    TXTR_free(sprites_library[SPRITE_WALLLIGHT]);
    TXTR_free(tilesets_library[TILESET_BASIC]);
}
