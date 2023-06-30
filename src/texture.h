#include <SDL2/SDL.h>

#include "global.h"

#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct texture {
    SDL_Surface *surface;
    int          id;
    const char  *filepath;
} texture_t;

texture_t* TXTR_read_from_file(const char *filepath);
void TXTR_free(texture_t* texture);

int TXTR_width(texture_t* texture);
int TXTR_height(texture_t* texture);


#endif
