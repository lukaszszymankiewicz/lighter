#include "def.h"
#include "hero.h"
#include "light.h"
#include "tile.h"
#include "timer.h"

#ifndef GAME_H
#define GAME_H

typedef struct game {
  bool loop;

  // objects
  hero_t *hero;
  tiles_list_t *tiles;
  light_t *light; // will be changed to some list of lights

  // timers
  game_timer_t *fps_timer;
  game_timer_t *cap_timer;
  int frame;
  int frame_ticks;

  // // input
  SDL_Event event;
  const Uint8 *keys_state; // current keyboard state
  Uint8 *keys_cooldown;    // past of keyboard state

} game_t;

#endif
