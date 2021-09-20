#include "sprites.h"
#include "primitives.h"
#include "macros.h"

#ifndef HERO_H
#define HERO_H

enum state { STANDING, WALKING, MAX_STATE };

typedef struct hero {
    int state;
    int x;
    int y;
    int direction;
    int frame;
    int frame_t;
    int x_vel;
    int y_vel;
    animation_sheet_t* sprites;
} hero_t;

hero_t* HERO_init(char* animation_sheet);
void HERO_draw(hero_t* hero_o);
void HERO_free(hero_t* hero_o);
int HERO_light_x(hero_t* hero_o);
int HERO_light_y(hero_t* hero_o);
void HERO_update(hero_t* hero_o);
void HERO_move(hero_t* hero_o, direction_t direction);

#endif
