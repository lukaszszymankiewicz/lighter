#include <stdlib.h>
#include "hero.h"
#include "sprites.h"


// this propably should lie in anothe file - but don`t worry with this now
enum light_sources {
    LANTERN = 0,
    FLASHLIGHT = 1,
    ALL = 2,
};

void HERO_load_frames(hero_t * hero_o) 
{
    hero_o->clips[ 0 ].x =  0;
    hero_o->clips[ 0 ].y =  0;
    hero_o->clips[ 0 ].w = 30;
    hero_o->clips[ 0 ].h = 45;

    //Set top right sprite
    hero_o->clips[ 1 ].x = 31;
    hero_o->clips[ 1 ].y =  0;
    hero_o->clips[ 1 ].w = 30;
    hero_o->clips[ 1 ].h = 45;
    
    //Set bottom left sprite
    hero_o->clips[ 2 ].x = 62;
    hero_o->clips[ 3 ].y =  0;
    hero_o->clips[ 2 ].w = 30;
    hero_o->clips[ 2 ].h = 45;

    //Set bottom right sprite
    hero_o->clips[ 3 ].x = 93;
    hero_o->clips[ 3 ].y =  0;
    hero_o->clips[ 3 ].w = 30;
    hero_o->clips[ 3 ].h = 45;
};

hero_t * HERO_init()
{
    hero_t* hero_o = (hero_t*)malloc(sizeof(hero_t));
    hero_o->x = 241;
    hero_o->y = 234;
    hero_o->state = 1;
    hero_o->light_source = LANTERN;

    hero_o->sprite = TXTR_init_texture("sprites/hero.png");

    hero_o->clips[ 0 ].x =  0;
    hero_o->clips[ 0 ].y =  0;
    hero_o->clips[ 0 ].w = 30;
    hero_o->clips[ 0 ].h = 45;

    //Set top right sprite
    hero_o->clips[ 1 ].x = 31;
    hero_o->clips[ 1 ].y =  0;
    hero_o->clips[ 1 ].w = 30;
    hero_o->clips[ 1 ].h = 45;
    
    //Set bottom left sprite
    hero_o->clips[ 2 ].x = 62;
    hero_o->clips[ 3 ].y =  0;
    hero_o->clips[ 2 ].w = 30;
    hero_o->clips[ 2 ].h = 45;

    //Set bottom right sprite
    hero_o->clips[ 3 ].x = 93;
    hero_o->clips[ 3 ].y =  0;
    hero_o->clips[ 3 ].w = 30;
    hero_o->clips[ 3 ].h = 45;

    return hero_o;
}

void HERO_change_light_source(int * light_source)
{
    *light_source = (*light_source + 1) % ALL;
}

void HERO_draw(hero_t * hero_o)
{
    TXTR_render_texture(
        hero_o->sprite,
        &((hero_o->clips)[hero_o->state]),
        hero_o->x - hero_o->sprite->width/8,
        hero_o->y - hero_o->sprite->height/2
    );
}

void HERO_free(hero_t * hero_o)
{
    TXTR_free(hero_o->sprite);
}
