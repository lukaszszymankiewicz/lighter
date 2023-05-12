#include <stdbool.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define KEYBOARD_CONTROLLER_DELAY 1

typedef struct controller {
    bool           available;      // flag showing if controller is available
    int            delay;          // delay (in frames) of how often controller state is updated
    int            len;            // number of available keys
    int            counter;        // delay counter
    Uint8          *state;         // current keyboard state
    Uint8          *old_state;     // past of keyboard state (1 frame before)
} controller_t;

extern controller_t *keyboard;

controller_t* CON_init();

void CON_update(controller_t* con);
void CON_free(controller_t* con);

bool CON_button_is_just_pressed(controller_t*, Uint8);
bool CON_button_is_just_released(controller_t*, Uint8);
bool CON_button_still_pressed(controller_t* con, Uint8 key);

#endif
