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

    hero_t* hero_o = (hero_t*)malloc(sizeof(hero_t));
    hero_o->state = 1;
    hero_o->x = 100;
    hero_o->y = 100;
    hero_o->sprite = NULL;

    // yeah - just faking it.
    hero_o->clips[0].x = 0; hero_o->clips[0].y = 0; hero_o->clips[0].w = 0; hero_o->clips[0].h = 0;
    hero_o->clips[1].x = 0; hero_o->clips[1].y = 0; hero_o->clips[1].w = 0; hero_o->clips[1].h = 0;
    hero_o->clips[2].x = 0; hero_o->clips[3].y = 0; hero_o->clips[2].w = 0; hero_o->clips[2].h = 0;
    hero_o->clips[3].x = 0; hero_o->clips[3].y = 0; hero_o->clips[3].w = 0; hero_o->clips[3].h = 0;

    // yeah - just faking it.
    light_t* light_o = (light_t*)malloc(sizeof(light_t));
    light_o->src = 0;
    light_o->angle = RIGHT_RAD;

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
    ck_assert_int_eq(hero_o->x, 80);
    ck_assert_int_eq(hero_o->y, 100);
    ck_assert_int_eq(hero_o->state, 1);

    // WHEN (MOVING RGHT)
    event.type = SDL_KEYDOWN;
    event.key.keysym.sym = SDLK_RIGHT;
    SDL_PushEvent(&event);
    EVNT_handle_events(&event, &loop, hero_o, light_o);
    ck_assert_int_eq(hero_o->x, 100);
    ck_assert_int_eq(hero_o->y, 100);
    ck_assert_int_eq(hero_o->state, 3);
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
