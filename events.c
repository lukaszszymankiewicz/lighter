#include "events.h"
#include "hero.h"

enum hero_states {
    HERO_STANDING_FRONT = 0,
    HERO_STANDING_LEFT = 1,
    HERO_STANDING_BACK = 2,
    HERO_STANDING_RIGHT = 3,
};

void EVNT_handle_events(SDL_Event *event, int *game_loop, hero_t * hero_o)
{
while(SDL_PollEvent(event) != 0) {
    if(event->type == SDL_QUIT) {
        *game_loop = 0;
    }

    // yeah. this is pretty ugly, but by now it is only for testing purposes
    else if(event->type == SDL_KEYDOWN) {
        switch(event->key.keysym.sym) {

            case SDLK_SPACE:
            // HERO_change_light_source(&(hero_o->light_source));
            break;

            case SDLK_RIGHT:
            hero_o->state = HERO_STANDING_RIGHT;
            hero_o->x += 20;
            break;

            case SDLK_LEFT:
            hero_o->state = HERO_STANDING_LEFT;
            hero_o->x -= 20;
            break;
            }
        }
    }
}
