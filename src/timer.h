#include <stdbool.h>

typedef struct timer
{
    int start_ticks;
    int paused_ticks;    // The ticks stored when the timer was paused
    bool paused;
    bool started;
} game_timer_t;

game_timer_t* TIMER_new();
void TIMER_start(game_timer_t* timer);
void TIMER_stop(game_timer_t* timer);
void TIMER_pause(game_timer_t* timer);
void TIMER_unpause(game_timer_t* timer);
int TIMER_get_ticks(game_timer_t* timer);
bool TIMER_is_started(game_timer_t* timer);
bool TIMER_is_paused(game_timer_t* timer);
void TIMER_free(game_timer_t* timer);
