#include "entity_manager.h"
#include "level.h"
#include "timer.h"

#ifndef GAME_H
#define GAME_H

#define GFX_INITIALIZERS_N 2 

enum GRAPHIC_OPTION {
    GRAPHIC_OFF,
    GRAPHIC_ON
};

typedef struct game {
  bool               loop;                // main game loop
  int                frame;               // current number of game frame
  int                frame_ticks;         // ticks passed
  int                max_frames; 
  int                draw_option;
  SDL_Event          event;               // SDL event (keyboard, mouse etc)
  level_t           *level;               // level
  game_timer_t      *fps_timer;           // FPS timer
  game_timer_t      *cap_timer;           // capping timer (for equall length in-game frame)
  entity_manager_t  *entity_manager;
} game_t;

void GAME_run(int level_id, int graphic_option, int max_frames);

#endif
