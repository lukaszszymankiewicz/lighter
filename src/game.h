#include "entity_manager.h"
#include "level.h"
#include "timer.h"

#ifndef GAME_H
#define GAME_H


typedef struct game {
  bool               loop;                  // main game loop
  int                frame;                 // current number of game frame
  int                frame_ticks;           // ticks passed

  SDL_Event          event;                // SDL event (keyboard, mouse etc)
} game_t;

void GAME_run();

#endif
