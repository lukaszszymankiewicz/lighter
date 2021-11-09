#include "timer.h"

// Basically the https://lazyfoo.net/tutorials/SDL/25_capping_frame_rate/index.php 

game_timer_t* TIMER_new() {
    game_timer_t* new_timer = NULL;
    new_timer = (game_timer_t*)malloc(sizeof(game_timer_t*));

    new_timer->start_ticks  = 0;
    new_timer->paused_ticks = 0;
    new_timer->paused       = false;
    new_timer->started      = false;

    return new_timer;
}

void TIMER_start(
    game_timer_t *timer
) {
    timer->started      = true;
    timer->paused       = false;
    timer->start_ticks  = SDL_GetTicks();
	timer->paused_ticks = 0;
}

void TIMER_stop(
    game_timer_t *timer
) {
    timer->started      = false;
    timer->paused       = false;
    timer->start_ticks  = 0;
	timer->paused_ticks = 0;
}

void TIMER_pause(
    game_timer_t *timer
) {
    //  If the timer is running and isn't already paused
    if (timer->started && !(timer->paused)) {
        timer->paused       = true;
        timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
        timer->start_ticks  = 0;
    }
}

void TIMER_unpause(
    game_timer_t *timer
) {
    //  If the timer is running and isn't already paused
    if (timer->started && timer->paused) {
        timer->paused       = false; // Unpause the timer
        timer->start_ticks  = SDL_GetTicks() - timer->paused_ticks; // Reset the starting ticks
        timer->paused_ticks = 0;
    }
}

int TIMER_get_ticks(
    game_timer_t* timer
) {
	int time = 0;

    if(timer->started) {
        if(timer->paused) {
            time = timer->paused_ticks; //Return the number of ticks when the timer was paused
        }
        else {
            time = SDL_GetTicks() - timer->start_ticks; //Return the current time minus the start time
        }
    }

    return time;
}

bool TIMER_is_started(
    game_timer_t *timer
) {
    return timer->started;
}

bool TIMER_is_paused(
    game_timer_t *timer
) {
    return timer->paused && timer->started;
}

void TIMER_free( 
    game_timer_t *timer
) {
    free(timer);
}
