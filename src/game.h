#include "def.h"
#include "timer.h"
#include "level.h"
#include "hero.h"
#include "light.h"

#ifndef GAME_H
#define GAME_H

extern int debug;

// main game struct containing most high-level game data
typedef struct game {
  bool           loop;           // main game loop
  int            frame;          // current number of game frame
  int            frame_ticks;    // ticks passed
  SDL_Event      event;          // SDL event (keyboard, mouse etc)
  hero_t        *hero;           // hero struct
  level_t       *level;          // level tiles 
  light_t       *light;          // current light objects
  game_timer_t  *fps_timer;      // FPS timer
  game_timer_t  *cap_timer;      // capping timer (for equall length in-game frame)
  const Uint8   *keys_state;     // current keyboard state
  Uint8         *keys_cooldown;  // past of keyboard state
} game_t;

#endif
