#include "sprites.h"

#ifndef HERO_H
#define HERO_H

typedef struct hero {
    int state;
    int x;
    int y;
    Texture* sprite;
    SDL_Rect clips[ 4 ];
} hero_t;

hero_t * HERO_init();
void HERO_draw(hero_t * hero_o);
void HERO_free(hero_t * hero_o);
int HERO_light_x(hero_t* hero_o);
int HERO_light_y(hero_t* hero_o);

#endif
