#include <stdlib.h>
#include "hero.h"
#include "sprites.h"

// do it dynamically!
int lightpos_x_corr[] = {9, 0};
int lightpos_y_corr[] = {11, 11};

hero_t * HERO_init()
{
    hero_t* hero_o = (hero_t*)malloc(sizeof(hero_t));
    hero_o->x = 160;  // yeah
    hero_o->y = 137;  //yeah
    hero_o->state = 1;

    hero_o->sprite = TXTR_init_texture("sprites/her2.png");

    hero_o->clips[ 0 ].x =  0;
    hero_o->clips[ 0 ].y =  0;
    hero_o->clips[ 0 ].w =  9;
    hero_o->clips[ 0 ].h = 20;

    //Set top right sprite
    hero_o->clips[ 1 ].x = 9;
    hero_o->clips[ 1 ].y = 0;
    hero_o->clips[ 1 ].w = 9;
    hero_o->clips[ 1 ].h = 20;

    return hero_o;
}

int HERO_light_x(hero_t* hero_o)
{
    return hero_o->x + lightpos_x_corr[hero_o->state];
}
int HERO_light_y(hero_t* hero_o)
{
    return hero_o->y + lightpos_y_corr[hero_o->state];
}

void HERO_draw(hero_t * hero_o)
{
    TXTR_render_texture(
        hero_o->sprite,
        &((hero_o->clips)[hero_o->state]),
        hero_o->x,
        hero_o->y
    );
}

void HERO_free(hero_t * hero_o)
{
    TXTR_free(hero_o->sprite);
}
