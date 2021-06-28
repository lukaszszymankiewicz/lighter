#include <SDL2/SDL.h>

#ifndef EVENTS_H
#define EVENTS_H

void EVNT_handle_events(SDL_Event *event, int *game_loop, int *hero_state, int *x, int *y);

#endif
