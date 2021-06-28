#include "events.h"

enum hero_states {
    HERO_STANDING_FRONT = 0,
    HERO_STANDING_LEFT = 1,
    HERO_STANDING_BACK = 2,
    HERO_STANDING_RIGHT = 3,
};

void EVNT_handle_events(SDL_Event *event, int *game_loop, int *hero_state, int *x, int *y)
{
while(SDL_PollEvent(event) != 0) {
    if(event->type == SDL_QUIT) {
        *game_loop = 0;
    }

    // yeah. this is pretty ugly, but by now it is only for testing purposes
    else if(event->type == SDL_KEYDOWN) {
        switch(event->key.keysym.sym) {
            case SDLK_UP:
            *hero_state = HERO_STANDING_BACK;
            (*y) -= 20;
            break;

            case SDLK_DOWN:
            *hero_state = HERO_STANDING_FRONT;
            (*y) += 20;
            break;

            case SDLK_RIGHT:
            *hero_state = HERO_STANDING_RIGHT;
            (*x) += 20;
            break;

            case SDLK_LEFT:
            *hero_state = HERO_STANDING_LEFT;
            (*x) -= 20;
            break;
            }
        }
    }
}
