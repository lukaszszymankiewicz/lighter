#include "entity_manager.h"
#include "level.h"
#include "timer.h"

#ifndef GAME_H
#define GAME_H


typedef struct game_config {
  int               max_frames;
  bool              use_gfx;
  int               level_id;
} game_config_t;

typedef struct game {
  bool               loop;                  // main game loop
  int                frame;                 // current number of game frame
  int                frame_ticks;           // ticks passed

  game_config_t      config;

  SDL_Event          event;                // SDL event (keyboard, mouse etc)
} game_t;

void GAME_run(game_config_t config);

#endif
