#include <SDL2/SDL_stdinc.h>

#include <stdbool.h>
#include <stdlib.h>

#include "controller.h"
#include "components.h"

void CON_clean(
) {
    for (int i=0; i<keyboard->len; i++) {
        keyboard->old_state[i]   = keyboard->state[i];
    }

    for (int i=0; i<keyboard->len; i++) {
        keyboard->old_state[i]   = 0;
    }
}

void CON_init(
) {
    keyboard              = NULL;
    keyboard              = (controller_t*)malloc(sizeof(controller_t));

    keyboard->available   = true;
    keyboard->delay       = KEYBOARD_CONTROLLER_DELAY;

    // get the len of SDL keyboard state array
    SDL_GetKeyboardState(&(keyboard->len));

    keyboard->state       = NULL;
    keyboard->old_state   = NULL;
    keyboard->counter     = 0;

    keyboard->state       = (Uint8*)malloc(sizeof(Uint8) * keyboard->len);
    keyboard->old_state   = (Uint8*)malloc(sizeof(Uint8) * keyboard->len);

    CON_clean(keyboard);
}

void CON_update(
) {
    if (keyboard->counter++ >= keyboard->delay) {

        keyboard->counter     = 0;
        Uint8* keyboard_state = NULL;
        keyboard_state        = (Uint8*)SDL_GetKeyboardState(NULL);

        for (int i=0; i<keyboard->len; i++) {
            keyboard->old_state[i] = keyboard->state[i];
            keyboard->state[i]     = keyboard_state[i];
        }
    }
}

bool CON_button_is_just_pressed(
    Uint8         key
) {
    return keyboard->state[key] && !(keyboard->old_state[key]);
}

bool CON_button_still_pressed(
    Uint8         key
) {
    return keyboard->state[key];
}

bool CON_button_is_just_released(
    Uint8         key
) {
    return !(keyboard->state[key]) && keyboard->old_state[key];
}

void CON_free(
) {
    if (keyboard->state) {
        free(keyboard->state);
    }
    keyboard->state       = NULL;

    if (keyboard->old_state) {
        free(keyboard->old_state);
    }
    keyboard->old_state   = NULL;

    free(keyboard);
    keyboard = NULL;
}
