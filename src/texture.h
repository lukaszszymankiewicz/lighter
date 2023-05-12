#include <SDL2/SDL.h>

#include "global.h"
#include "assets.h"

#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct texture {
    SDL_Surface *surface;
    int          id;
    const char  *filepath;
} texture_t;

extern texture_t         *sprites[ASSET_SPRITE_ALL];

texture_t* TXTR_read_from_file(const char *filepath);

int TXTR_width(texture_t* texture);
int TXTR_height(texture_t* texture);

void TXTR_read_all_sprites();

#endif
