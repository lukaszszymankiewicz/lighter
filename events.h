#include <SDL2/SDL.h>
#include "hero.h"

#ifndef EVENTS_H
#define EVENTS_H

void EVNT_handle_events(SDL_Event *event, int *game_loop, hero_t * hero_o);

#endif
