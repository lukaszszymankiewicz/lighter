#include <stdbool.h>

#include "library.h"
#include "util.h"

int LIB_check_modules(
) {
    for (int i=0; i<GFX_MODULE_ALL; i++) {
        if (gfx_module_library[i]->status == false) {
            printf("ERROR %c%d: ", gfx_module_library[i]->error_symbol, i);
            printf("%s cannot be initialied! \n", gfx_module_library[i]->name);
            return 0;
        }
    }
    return 1;
}

void LIB_init_all_modules(
) {
    LIB_init_gfx_modules();
}

void LIB_create_all(
) {
    LIB_create_entity_library();
    LIB_create_wobble_library();
    LIB_create_lightsources_library();
    LIB_create_sprites_library();
    LIB_create_levels_library();
    LIB_create_tile_library();
    LIB_create_tilesets_library();
    // LIB_create_shaders_library();
}

void LIB_free_all(
) {
    TXTR_free(sprites_library[SPRITE_HERO]);
    TXTR_free(sprites_library[SPRITE_LIGHTER]);
    TXTR_free(sprites_library[SPRITE_WALLLIGHT]);
    TXTR_free(tilesets_library[TILESET_BASIC]);
}
