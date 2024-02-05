#include "library.h"


void LIB_init(
) {
    printf("[RESOURCES]\n");
    LIB_create_entity_library();
    LIB_create_wobble_library();
    LIB_create_lightsources_library();
    LIB_create_sprites_library();
    LIB_create_levels_library();
    LIB_create_tile_library();
    LIB_create_tilesets_library();
    LIB_create_shaders_library();
}

void LIB_free_all(
) {
    GFX_free_texture(sprites_library[SPRITE_HERO]);
    GFX_free_texture(sprites_library[SPRITE_LIGHTER]);
    GFX_free_texture(sprites_library[SPRITE_WALLLIGHT]);
    GFX_free_texture(tilesets_library[TILESET_BASIC]);
}
