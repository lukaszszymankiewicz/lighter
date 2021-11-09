#include <check.h>
#include "../src/hero.h"


START_TEST (HERO_init_check)
{
    // GIVEN
    hero_t* hero_o = HERO_init("../sprites/her2.png");

    // WHEN
    ck_assert_ptr_nonnull(hero_o);
    ck_assert_ptr_nonnull(hero_o->sprites);
    ck_assert_int_eq(hero_o->state, 0);
    ck_assert_int_eq(hero_o->x, 384);
    ck_assert_int_eq(hero_o->y, 384);
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
    hero_t* hero_o = HERO_init("../sprites/her2.png");

    // WHEN
    hero_o->x = 100;
    hero_o->y = 100;
    
    // THEN
    hero_o->direction = 0;
    x = HERO_light_x(hero_o);
    y = HERO_light_y(hero_o);
    ck_assert_int_eq(x, 165);
    ck_assert_int_eq(y, 155);

    hero_o->direction = 1;
    x = HERO_light_x(hero_o);
    y = HERO_light_y(hero_o);
    ck_assert_int_eq(x, 160);
    ck_assert_int_eq(y, 155);

    // CLEANUP
    HERO_free(hero_o);
}
END_TEST

START_TEST (HERO_update_check)
{
    // GIVEN
    hero_t* hero_o = HERO_init("../sprites/her2.png");

    hero_o->sprites->animations[hero_o->state].delay = 2;
    hero_o->sprites->animations[hero_o->state].len = 2;

    // WHEN && THEN
    ck_assert_int_eq(hero_o->frame_t, 0);
    ck_assert_int_eq(hero_o->frame, 0);

    HERO_update(hero_o);                  // frame counter up, sprite not changed
    ck_assert_int_eq(hero_o->frame_t, 1);
    ck_assert_int_eq(hero_o->frame, 0);

    HERO_update(hero_o);                 // frame counter up, sprite changed, frame counter zeroed
    ck_assert_int_eq(hero_o->frame_t, 0);
    ck_assert_int_eq(hero_o->frame, 1);

    HERO_update(hero_o);                  // frame counter up, sprite not changed
    ck_assert_int_eq(hero_o->frame_t, 1);
    ck_assert_int_eq(hero_o->frame, 1);

    HERO_update(hero_o);                  // frame counter up, sprite changes, frame counter zeroed
    ck_assert_int_eq(hero_o->frame_t, 0);
    ck_assert_int_eq(hero_o->frame, 0);

    // CLEANUP
    HERO_free(hero_o);
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

    suite_add_tcase(s, tc_core);

    return s;
}
