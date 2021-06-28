#include <SDL2/SDL.h>

#ifndef SPRITES_H
#define SPRITES_H

typedef struct texture {
    SDL_Texture* surface;
    int width;
    int height;
} Texture;

void TXTR_draw_hero(int x, int y, int hero_state);
void TXTR_draw_light(int x, int y);
void TXTR_load_frames();
void TXTR_free_all();

#endif
