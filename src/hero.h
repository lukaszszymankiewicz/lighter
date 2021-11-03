#include "sprites.h"
#include "primitives.h"
#include "macros.h"

#ifndef HERO_H
#define HERO_H

#define MAX_VEL     5
#define MOVE_POWUH  2
#define X_FRICTION  1

enum state { STANDING, WALKING, MAX_STATE };

typedef struct hero {
    int state;
    int x;
    int y;
    int view_x;
    int view_y;
    int direction;
    int frame;
    int frame_t;
    int x_vel;
    animation_sheet_t* sprites;
} hero_t;

hero_t* HERO_init(char* animation_sheet);
void HERO_draw(hero_t* hero_o);
void HERO_free(hero_t* hero_o);
void HERO_update(hero_t* hero_o);
void HERO_move(hero_t* hero_o, direction_t direction);
int HERO_light_x(hero_t* hero_o);
int HERO_light_y(hero_t* hero_o);
void HERO_move_up(hero_t *hero_o);

#endif
