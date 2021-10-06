#include "def.h"
#include "hero.h"
#include "gfx.h"
#include "primitives.h"

static int MAX_VEL = 5;
static int MOVE_POWUH = 2;
static int X_FRICTION = 1;

// do it dynamically!
int lightpos_x_corr[] = {5, 0};
int lightpos_y_corr[] = {11, 11};

hero_t* HERO_init(char* animation_sheet)
{
    hero_t* hero_o = (hero_t*)malloc(sizeof(hero_t));

    hero_o->x = 384;
    hero_o->y = 384;

    hero_o->state = STANDING;
    hero_o->direction = LEFT;

    if (animation_sheet == NULL)
    { hero_o->sprites = TXTR_init_animation_sheet("sprites/her2.png", MAX_STATE);}
    else
    { hero_o->sprites = TXTR_init_animation_sheet(animation_sheet, MAX_STATE);}

    hero_o->frame=0;
    hero_o->frame_t=0;

    hero_o->x_vel=0;
    hero_o->y_vel=0;

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
        WALKING,
        (int[]){0, 9},
        (int[]){20, 20},
        (int[]){9, 9},
        (int[]){20, 20},
        5,
        2
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
    // ANIMATIONS
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
    if (hero_o->x_vel == 0) { hero_o->state=STANDING; }

    // MOVEMENT
    if (hero_o->direction == LEFT)
    {
        hero_o->x -= hero_o->x_vel;
    }
    else
    {
        hero_o->x += hero_o->x_vel;
    }

    // PHYSICS
    hero_o->x_vel = MAX(0, hero_o->x_vel-X_FRICTION);
}

void HERO_move(hero_t* hero_o, direction_t direction) {
    hero_o->state = WALKING;

    if (direction == hero_o->direction) {
        hero_o->x_vel = MIN(MAX_VEL, hero_o->x_vel + MOVE_POWUH);
    }
    else {
        hero_o->x_vel = MOVE_POWUH;
    }
    hero_o->direction = direction;
}


SDL_Rect* HERO_current_frame(hero_t* hero_o) {
    return &(hero_o->sprites->animations[hero_o->state].frames[hero_o->frame]);
}

void HERO_draw(hero_t* hero_o) {
    GFX_render_texture(
        hero_o->sprites->texture,
        HERO_current_frame(hero_o),
        SCREEN_WIDTH / 2,
        144,
        hero_o->direction
    );
}

void HERO_free(hero_t * hero_o) {
    TXTR_free_animation_sheet(hero_o->sprites);
}
