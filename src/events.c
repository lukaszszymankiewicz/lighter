#include "events.h"
#include "config.h"
#include "hero.h"


void EVNT_handle_events(SDL_Event *event, int *game_loop, hero_t * hero_o, light_t * light_o)
{
while(SDL_PollEvent(event) != 0) {
    if(event->type == SDL_QUIT) {
        *game_loop = 0;
    }

    // yeah. this is pretty ugly, but by now it is only for testing purposes
    else if(event->type == SDL_KEYDOWN) {
        switch(event->key.keysym.sym) {
            
            case SDLK_SPACE:
            LIG_change_source(light_o);
            break;

            case SDLK_RIGHT:
            hero_o->direction = RIGHT;
            hero_o->x += 20;
            light_o->angle = RIGHT_RAD;
            break;

            case SDLK_LEFT:
            hero_o->direction = LEFT;
            hero_o->x -= 20;
            light_o->angle = LEFT_RAD;
            break;
            }
        }
    }
}
