#include "global.h"
#include "primitives.h"
#include "sprites.h"

#ifndef HERO_H
#define HERO_H

#define MAX_VEL          5
#define MOVE_POWUH       2
#define X_FRICTION       1
#define Y_FRICTION       1
#define JUMP_POWUH       10

enum state { STANDING, WALKING, JUMPING, FALLING_DOWN, MAX_STATE };

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
    int y_vel;
    animation_sheet_t* sprites;
} hero_t;

hero_t* HERO_init(char* animation_sheet);
void HERO_draw(hero_t* hero_o);
void HERO_free(hero_t* hero_o);
int HERO_light_x(hero_t* hero_o);
int HERO_light_y(hero_t* hero_o);

void HERO_move(hero_t* hero_o, direction_t direction);
void HERO_jump(hero_t *hero_o);

void HERO_check_collision(hero_t *hero, segment_t *obstacles);
void HERO_update_sprite(hero_t *hero_o);
void HERO_update_state(hero_t *hero);
void HERO_update_pos_due_to_velocity(hero_t *hero);
void HERO_update_friction(hero_t *hero);

#endif
