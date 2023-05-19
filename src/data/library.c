#include "library.h"


void LIB_create_all(
) {
    LIB_create_entity_library();
    LIB_create_wobble_library();
    LIB_create_lightsources_library();
    LIB_create_sprites_library();
}

void LIB_free_all(
) {
    GFX_free_texture(sprites_library[SPRITE_HERO]);
    GFX_free_texture(sprites_library[SPRITE_LIGHTER]);
    GFX_free_texture(sprites_library[SPRITE_WALLLIGHT]);
    SRC_free_wobble(wobble_library[WOBBLE_NO]);
    SRC_free_wobble(wobble_library[WOBBLE_STABLE]);
    SRC_free_wobble(wobble_library[WOBBLE_WALKING]);
    SRC_free_lightsource(lighsources_library[LIGHTSOURCE_LIGHTER]);
    SRC_free_lightsource(lighsources_library[LIGHTSOURCE_WALLLIGHT]);
}
