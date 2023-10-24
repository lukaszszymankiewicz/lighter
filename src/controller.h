#include <stdint.h>
#include <stdbool.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define KEYBOARD_CONTROLLER_DELAY 1

typedef struct controller {
    bool           available;      // flag showing if controller is available
    int            delay;          // delay (in frames) of how often controller state is updated
    int            len;            // number of available keys
    int            counter;        // delay counter
    uint8_t       *state;          // current keyboard state
    uint8_t       *old_state;      // past of keyboard state (1 frame before)
} controller_t;

void CON_init();
void CON_update();
void CON_free();

bool CON_button_is_just_pressed(uint8_t key);
bool CON_button_is_just_released(uint8_t key);
bool CON_button_still_pressed(uint8_t key);

#endif
