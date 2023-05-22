#include <stdbool.h>

#include "library.h"


void LIB_create_all(
) {
    LIB_create_entity_library();
    LIB_create_wobble_library();
    LIB_create_lightsources_library();
    LIB_create_sprites_library();
    LIB_create_levels_library();
}

bool LIB_validate(
) {
    for (int i=0; i<ENTITY_ALL; i++) {
        if (entity_library[i] == NULL) { return false; }     
    }

    return true;
}

void LIB_free_all(
) {
    TXTR_free(sprites_library[SPRITE_HERO]);
    TXTR_free(sprites_library[SPRITE_LIGHTER]);
    TXTR_free(sprites_library[SPRITE_WALLLIGHT]);
    SRC_free_wobble(wobble_library[WOBBLE_NO]);
    SRC_free_wobble(wobble_library[WOBBLE_STABLE]);
    SRC_free_wobble(wobble_library[WOBBLE_WALKING]);
    SRC_free_lightsource(lighsources_library[LIGHTSOURCE_LIGHTER]);
    SRC_free_lightsource(lighsources_library[LIGHTSOURCE_WALLLIGHT]);
    LVL_free(levels_library[LEVEL_SAMPLE]);
}
