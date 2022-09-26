#include <check.h>
#include "../src/segment.h"
#include "../src/hero.h"


START_TEST (HERO_init_check)
{
    // GIVEN
    hero_t* hero_o = NULL;
    hero_o         = HERO_init(10, 10);

    // WHEN
    ck_assert_ptr_nonnull(hero_o);
    ck_assert_ptr_nonnull(hero_o->sprites);
    ck_assert_int_eq(hero_o->state, 0);
    ck_assert_int_eq(hero_o->x, 320);
    ck_assert_int_eq(hero_o->y, 320);
    ck_assert_int_eq(hero_o->view_x, 160);
    ck_assert_int_eq(hero_o->view_y, 144);
    ck_assert_int_eq(hero_o->direction, 1);
    ck_assert_int_eq(hero_o->frame, 0);
    ck_assert_int_eq(hero_o->frame_t, 0);

    // CLEANUP
    HERO_free(hero_o);
}
END_TEST

START_TEST (HERO_light_x_and_y_check)
{
    // GIVEN
    int x, y;
    hero_t* hero_o = HERO_init(100, 100);

    // THEN
    hero_o->direction = 0;
    x = HERO_light_x(hero_o);
    y = HERO_light_y(hero_o);
    ck_assert_int_eq(x, 165);
    ck_assert_int_eq(y, 155);

    hero_o->direction = 1;
    x = HERO_light_x(hero_o);
    y = HERO_light_y(hero_o);
    ck_assert_int_eq(x, 161);
    ck_assert_int_eq(y, 155);

    // CLEANUP
    HERO_free(hero_o);
}
END_TEST

START_TEST (HERO_update_check)
{
    // GIVEN
    hero_t* hero_o = HERO_init(100, 100);

    hero_o->sprites->animations[hero_o->state]->delay = 2;
    hero_o->sprites->animations[hero_o->state]->len = 2;

    // WHEN && THEN
    ck_assert_int_eq(hero_o->frame_t, 0);
    ck_assert_int_eq(hero_o->frame, 0);

    HERO_update_sprite(hero_o);                  // frame counter up, sprite not changed
    ck_assert_int_eq(hero_o->frame_t, 1);
    ck_assert_int_eq(hero_o->frame, 0);

    HERO_update_sprite(hero_o);                  // frame counter up, sprite not changed
    ck_assert_int_eq(hero_o->frame_t, 0);
    ck_assert_int_eq(hero_o->frame, 1);

    HERO_update_sprite(hero_o);                  // frame counter up, sprite not changed
    ck_assert_int_eq(hero_o->frame_t, 1);
    ck_assert_int_eq(hero_o->frame, 1);

    HERO_update_sprite(hero_o);                  // frame counter up, sprite changes, frame counter zeroed
    ck_assert_int_eq(hero_o->frame_t, 0);
    ck_assert_int_eq(hero_o->frame, 0);

    // CLEANUP
    HERO_free(hero_o);
}
END_TEST

START_TEST (HERO_colision_check_positive)
{
    // GIVEN
    // hero as it is have hitbox already in it
    // hero->x = 100;
    // hero->y = 100;
    // hitbox = {0, 0, 9, 20},
    // overall hitbox = { 100, 100, 109, 120 } + 1 (velocity)

    // init hero at (0, 0) as this value will be overwritten below
    hero_t* hero = HERO_init(0, 0);
    segment_t *obstacles = NULL;

    // CASE 1
    obstacles = NULL;
    SEG_push(&obstacles, 105, 80, 105, 130);
    hero->view_x = 100; hero->view_y = 100; hero->x_vel = 1; hero->direction = RIGHT;
    HERO_check_collision(hero, obstacles);
    ck_assert_int_eq(hero->x, 105);

    // CASE 2 (lower edge)
    obstacles = NULL;
    SEG_push(&obstacles, 105, 80, 105, 100);
    hero->x = 100; hero->y = 100; hero->x_vel = 1;
    HERO_check_collision(hero, obstacles);
    ck_assert_int_eq(hero->x, 100);

    // CASE 3 (lower edge)
    obstacles = NULL;
    SEG_push(&obstacles, 0, 110, 200, 110);
    hero->x = 100; hero->y = 100; hero->y_vel = 1.0;
    HERO_check_collision(hero, obstacles);
    ck_assert_int_eq(hero->y, 90);
}
END_TEST

START_TEST (HERO_colision_check_negatvie)
{
    // GIVEN
    // hero as it is have hitbox already in it
    // hero->x = 100;
    // hero->y = 100;
    // hitbox = {0, 0, 9, 20},
    // overall hitbox = { 100, 100, 109, 120 } + 1 (velocity)

    // CASE 1
    hero_t* hero = HERO_init(100, 100);
    segment_t *obstacles = NULL;
    SEG_push(&obstacles, 105, 80, 105, 90);

    // adding some velocity for collision
    hero->view_x = 100;
    hero->view_y = 100;
    hero->x_vel = 1;

    // WHEN && THEN
    HERO_check_collision(hero, obstacles);

    // no collision so velocity should be not constrained
    ck_assert_int_eq(hero->x_vel, 1);

    // CASE 2
    obstacles = NULL;
    SEG_push(&obstacles, 0, 110, 200, 110);  // hero is standing on plaftorm

    // adding some velocity for collision
    hero->x = 100;
    hero->y = 100;
    hero->view_x = 100;
    hero->view_y = 100;
    hero->y_vel = 1;

    // WHEN && THEN
    HERO_check_collision(hero, obstacles);

    // hero fall of the platform, so he should stop
    ck_assert_int_eq(hero->y_vel, 0);
    ck_assert_int_eq(hero->y, 90);
}
END_TEST

Suite* hero_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("hero");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, HERO_init_check);
    tcase_add_test(tc_core, HERO_light_x_and_y_check);
    tcase_add_test(tc_core, HERO_update_check); 
    tcase_add_test(tc_core, HERO_colision_check_negatvie); 
    // tcase_add_test(tc_core, HERO_colision_check_positive);

    suite_add_tcase(s, tc_core);

    return s;
}
