#include "sprites.h"

#ifndef HERO_H
#define HERO_H

typedef struct hero {
    int state;
    int x;
    int y;
    int light_source;
    Texture * sprite;
    SDL_Rect clips[ 4 ];
} hero_t;

hero_t * HERO_init();
void HERO_change_light_source(int * light_source);
void HERO_draw(hero_t * hero_o);
void HERO_free(hero_t * hero_o);

#endif
