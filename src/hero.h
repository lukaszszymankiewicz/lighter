#include "sprites.h"

#ifndef HERO_H
#define HERO_H

enum state { STANDING, IDLE, MAX_STATE };
enum direction { RIGHT, LEFT };

typedef struct hero {
    int state;
    int x;
    int y;
    int direction;
    int frame;
    int frame_t;
    animation_sheet_t* sprites;
} hero_t;

hero_t* HERO_init(char* animation_sheet);
void HERO_draw(hero_t* hero_o);
void HERO_free(hero_t* hero_o);
int HERO_light_x(hero_t* hero_o);
int HERO_light_y(hero_t* hero_o);
void HERO_update(hero_t* hero_o);

#endif
