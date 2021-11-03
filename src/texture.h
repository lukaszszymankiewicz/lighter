#include <SDL2/SDL.h>

#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct texture {
    SDL_Texture *surface;
    int          width;
    int          height;
} texture_t;

#endif
