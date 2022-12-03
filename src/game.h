#include "global.h"
#include "timer.h"
#include "level.h"
#include "entity_manager.h"

#ifndef GAME_H
#define GAME_H

typedef struct game {
  bool               loop;                // main game loop
  int                frame;               // current number of game frame
  int                frame_ticks;         // ticks passed
  SDL_Event          event;               // SDL event (keyboard, mouse etc)
  level_t           *level;               // level
  game_timer_t      *fps_timer;           // FPS timer
  game_timer_t      *cap_timer;           // capping timer (for equall length in-game frame)
  entity_manager_t  *entity_manager;
} game_t;

void GAME_run();

#endif
