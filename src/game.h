#include <stdbool.h>
#include "config.h"
#include "events.h"
#include "gfx.h"
#include "hero.h"
#include "level.h"
#include "light.h"
#include "timer.h"

#ifndef LIGHTER_H
#define LIGHTER_H

typedef struct game {
    bool loop;
    int frame;
    int frame_ticks;
    hero_t* hero;
    tiles_list_t* tiles;         // level will be loaded etc, not it is static and hardwired
    light_t* light;              // will be changed to some list of lights
    game_timer_t* fps_timer;
    game_timer_t* cap_timer;
    SDL_Event event;
} game_t;


#endif
