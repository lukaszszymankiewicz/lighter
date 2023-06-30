#include <SDL2/SDL.h>

#include <SDL2/SDL_stdinc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "controller.h"

controller_t *keyboard = NULL;

void CON_clean(
    controller_t* con
) {
    for (int i=0; i<con->len; i++) {
        con->old_state[i]   = con->state[i];
    }

    for (int i=0; i<con->len; i++) {
        con->old_state[i]   = 0;
    }
}

controller_t* CON_init(
) {
    controller_t* con          = NULL;
    con                        = (controller_t*)malloc(sizeof(controller_t));

    con->available             = true;
    con->delay                 = KEYBOARD_CONTROLLER_DELAY;

    // get the len of SDL keyboard state array
    SDL_GetKeyboardState(&(con->len));

    con->state       = NULL;
    con->old_state   = NULL;
    con->counter     = 0;

    con->state       = (Uint8*)malloc(sizeof(Uint8) * con->len);
    con->old_state   = (Uint8*)malloc(sizeof(Uint8) * con->len);

    CON_clean(con);

    return con;
}

void CON_update(
    controller_t* con
) {
    if (con->counter++ >= con->delay) {

        con->counter          = 0;
        Uint8* keyboard_state = NULL;
        keyboard_state        = (Uint8*)SDL_GetKeyboardState(NULL);

        for (int i=0; i<con->len; i++) {
            con->old_state[i] = con->state[i];
            con->state[i]     = keyboard_state[i];
        }
    }
}

bool CON_button_is_just_pressed(
    controller_t *con,
    Uint8         key
) {
    return con->state[key] && !(con->old_state[key]);
}

bool CON_button_still_pressed(
    controller_t *con,
    Uint8         key
) {
    return con->state[key];
}

bool CON_button_is_just_released(
    controller_t *con,
    Uint8         key
) {
    return !(con->state[key]) && con->old_state[key];
}

void CON_free(
    controller_t* con
) {
    if (con->state) {
        free(con->state);
    }
    con->state       = NULL;

    if (con->old_state) {
        free(con->old_state);
    }
    con->old_state   = NULL;

    free(con);
    con = NULL;
}
