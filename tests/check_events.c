#include <SDL2/SDL.h>
#include <check.h>
#include <stdlib.h>
#include "../src/config.h"
#include "../src/events.h"
#include "../src/hero.h"
#include "../src/light.h"


START_TEST (EVNT_handle_events_check)
{
    // GIVEN
    int loop = 1;

    // hero_t* hero_o = HERO_init();
    light_t* light_o = LIG_init();

    // sprites are really unused here, so there are not initialized really
    hero_t* hero_o = (hero_t*)malloc(sizeof(hero_t));
    hero_o->x = 160;
    hero_o->y = 137;
    hero_o->state = STANDING;
    hero_o->direction = LEFT;
    hero_o->frame=0;
    hero_o->frame_t=0;

    // WHEN (QUITING)
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);

    EVNT_handle_events(&event, &loop, hero_o, light_o);
    ck_assert_int_eq(loop, 0);

    // WHEN (LIGHT CHANGE)
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_SPACE;
    SDL_PushEvent(&event);
    EVNT_handle_events(&event, &loop, hero_o, light_o);
    ck_assert_int_eq(light_o->src_num, 1);

    // WHEN (LIGHT CHANGE BACK)
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_SPACE;
    SDL_PushEvent(&event);
    EVNT_handle_events(&event, &loop, hero_o, light_o);
    ck_assert_int_eq(light_o->src_num, 0);

    // WHEN (MOVING LEFT)
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_LEFT;
    SDL_PushEvent(&event);
    EVNT_handle_events(&event, &loop, hero_o, light_o);
    ck_assert_int_eq(hero_o->x, 140);
    ck_assert_int_eq(hero_o->y, 137);
    ck_assert_int_eq(hero_o->state, 0);

    // WHEN (MOVING RGHT)
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_RIGHT;
    SDL_PushEvent(&event);
    EVNT_handle_events(&event, &loop, hero_o, light_o);
    ck_assert_int_eq(hero_o->x, 160);
    ck_assert_int_eq(hero_o->y, 137);
    ck_assert_int_eq(hero_o->state, 0);

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
