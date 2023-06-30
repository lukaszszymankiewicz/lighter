#include "texture.h"

int TXTR_width(
    texture_t* texture
) {
    if (!texture || !(texture->surface)) {
        return -1;
    } else {
        return texture->surface->w;
    }
}

int TXTR_height(
    texture_t* texture
) {
    if (!texture || !(texture->surface)) {
        return -1;
    } else {
        return texture->surface->h;
    }
}

