#include <SDL2/SDL.h>
#include <check.h>
#include <stdlib.h>
#include "../src/global.h"
#include "../src/events.h"
#include "../src/hero.h"
#include "../src/light.h"


START_TEST (EVNT_handle_events_check)
{
    // GIVEN
    int loop = 1;

    hero_t* hero_o = NULL;
    light_t* light_o = LIG_init();

    // sprites are really unused here, so some way around is used to get proper hero struct without
    // texture initialised
    hero_o = (hero_t*)malloc(sizeof(hero_t));
    hero_o->x = 100;
    hero_o->y = 100;
    hero_o->state = STANDING;
    hero_o->direction = LEFT;
    hero_o->frame=0;
    hero_o->frame_t=0;

    // only things needed are initialised in game struct
    game_t* game = (game_t*)malloc(sizeof(game_t));
    game->loop = true;
    game->hero = hero_o;
    game->light = LIG_init();
    game->frame = 0;
    game->keys_state = (Uint8 *)malloc(sizeof(Uint8) * MAX_KEYS);
    game->keys_cooldown = NULL;
    game->keys_cooldown = (Uint8 *)malloc(sizeof(Uint8) * MAX_KEYS);

    SDL_Event e;
    game->event = e;
 
    // WHEN (QUITING)
    game->event.type = SDL_QUIT;
    SDL_PushEvent(&(game->event));
    EVNT_handle_events(game);
    ck_assert_int_eq(game->loop, 0);

    // WHEN (LIGHT CHANGE)
    Uint8* new_keys = (Uint8 *)malloc(sizeof(Uint8) * MAX_KEYS);
    new_keys[SDL_SCANCODE_SPACE] = 2;  // exit button pressed
    game->keys_state = new_keys;
    SDL_PushEvent(&(game->event));
    EVNT_handle_events(game);
    ck_assert_int_eq(light_o->src_num, 0);
    free(new_keys);

    // WHEN (LIGHT CHANGE BACK)
    game->event.type = SDL_KEYDOWN;
    game->event.key.keysym.sym = SDL_SCANCODE_SPACE;
    SDL_PushEvent(&(game->event));
    EVNT_handle_events(game);
    ck_assert_int_eq(light_o->src_num, 0);

    // WHEN (MOVING UP)
    game->event.type = SDL_KEYDOWN;
    game->event.key.keysym.sym = SDL_SCANCODE_UP;
    EVNT_handle_events(game);
    ck_assert_int_eq(hero_o->state, 2);

    // WHEN (MOVING DOWN)
    game->event.type = SDL_KEYDOWN;
    game->event.key.keysym.sym = SDL_SCANCODE_DOWN;
    EVNT_handle_events(game);
    ck_assert_int_eq(hero_o->state, 2);

    // CLEANING
    free(hero_o);
    LIG_free(light_o);
}
END_TEST

Suite * events_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("events");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, EVNT_handle_events_check);

    suite_add_tcase(s, tc_core);

    return s;
}
