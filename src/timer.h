#include <stdbool.h>

#include "global.h"

#ifndef TIMER_H
#define TIMER_H

typedef struct game_timer {
    int start_ticks;
    int paused_ticks;
    bool paused;
    bool started;
} game_timer_t;

game_timer_t *TIMER_new();

int TIMER_get_ticks(game_timer_t *timer);

void TIMER_start(game_timer_t *timer);
void TIMER_stop(game_timer_t *timer);
void TIMER_pause(game_timer_t *timer);
void TIMER_unpause(game_timer_t *timer);
void TIMER_free(game_timer_t *timer);

void TIMER_init_fps_timer();
void TIMER_init_cap_timer();

bool TIMER_is_started(game_timer_t *timer);
bool TIMER_is_paused(game_timer_t *timer);

#endif
