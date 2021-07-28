#include <stdbool.h>
#include <stdlib.h>
#include "hero.h"
#include "sprites.h"

// do it dynamically!
int lightpos_x_corr[] = {5, 0};
int lightpos_y_corr[] = {11, 11};

hero_t* HERO_init(char* animation_sheet)
{
    hero_t* hero_o = (hero_t*)malloc(sizeof(hero_t));

    hero_o->x = 160;          // yeah
    hero_o->y = 137;          //yeah

    hero_o->state = STANDING;
    hero_o->direction = LEFT;

    if (animation_sheet == NULL)
    { hero_o->sprites = TXTR_init_animation_sheet("sprites/her2.png", MAX_STATE);}
    else
    { hero_o->sprites = TXTR_init_animation_sheet(animation_sheet, MAX_STATE);}

    hero_o->frame=0;
    hero_o->frame_t=0;

    TXTR_push_animation(
        hero_o->sprites,
        STANDING,
        (int[]){0, 9},
        (int[]){0, 0},
        (int[]){9, 9},
        (int[]){20, 20},
        20,
        2
    );

    TXTR_push_animation(
        hero_o->sprites,
        IDLE,
        (int[]){0},
        (int[]){20},
        (int[]){9},
        (int[]){20},
        20,
        1
    );

    return hero_o;
}

int HERO_light_x(hero_t* hero_o)
{
    return hero_o->x + lightpos_x_corr[hero_o->direction];
}

int HERO_light_y(hero_t* hero_o)
{
    return hero_o->y + lightpos_y_corr[hero_o->direction];
}

void HERO_update(hero_t* hero_o)
{
    hero_o->frame_t++;
    int del = hero_o->sprites->animations[hero_o->state].delay;
    int len = hero_o->sprites->animations[hero_o->state].len;

    if (hero_o->frame_t >= del)
    {
        hero_o->frame_t=0;
        hero_o->frame++; 

        if (hero_o->frame >= len)
        {
            hero_o->frame=0;
        }
    }
}

void HERO_draw(hero_t* hero_o)
{
    TXTR_render_texture(
        hero_o->sprites->texture,
        &(hero_o->sprites->animations[hero_o->state].frames[hero_o->frame]),
        hero_o->x,
        hero_o->y,
        hero_o->direction
    );
}

void HERO_free(hero_t * hero_o)
{
    TXTR_free_animation_sheet(hero_o->sprites);
}
