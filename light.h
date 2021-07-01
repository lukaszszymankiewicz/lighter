#include "tile.h"
#include "sprites.h"

#ifndef LIGHT_H
#define LIGHT_H

typedef struct light {
    int source;
    Texture * sprite;
} light_t;

void LIG_draw_light_effect(light_t * light_o, int hero_x, int hero_y, tiles_list_t * tiles);
light_t * LIG_init();
void LIG_free(light_t * light_o);
void LIG_change_source(int * light_source);

#endif
