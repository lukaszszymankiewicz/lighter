#include "light.h"
#include "game.h"

#ifndef EVENTS_H
#define EVENTS_H

#define SATURATE(a, b) ((b) ? (a = a + b) : (0))
#define KEY_PRESSED(a) (game->keys_cooldown[a] > keyboard_delay[a])
#define N_LEGAL_KEYS 5

void EVNT_init();
void EVNT_handle_events(game_t *game);

#endif
