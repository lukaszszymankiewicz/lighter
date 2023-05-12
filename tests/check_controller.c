#include <check.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../src/controller.h"

START_TEST (CON_init_check)
{
    controller_t* con = NULL;
    con = CON_init();

    ck_assert_int_eq(con->available, 1);
    ck_assert_int_eq(con->delay,     1);
    ck_assert_int_eq(con->counter,   0);
    ck_assert_int_gt(con->len, 1);
    ck_assert_ptr_nonnull(con->state);
    ck_assert_ptr_nonnull(con->old_state);
}
END_TEST

START_TEST (CON_update_delay_check)
{
    controller_t* con = NULL;
    con               = CON_init();
    con->counter      = 0;

    // artifically create some delay on keyboard
    con->delay = 1;

    CON_update(con);
    ck_assert_int_eq(con->delay,     1);
    ck_assert_int_eq(con->counter,   1);

    CON_update(con);
    ck_assert_int_eq(con->delay,     1);
    ck_assert_int_eq(con->counter,   0);

    CON_update(con);
    ck_assert_int_eq(con->delay,     1);
    ck_assert_int_eq(con->counter,   1);

}
END_TEST

START_TEST (CON_update_read_keyboard)
{
    controller_t* con = NULL;
    con               = CON_init();
    Uint8 key         = SDL_SCANCODE_S;
    con->delay        = 0;

    // artifically press one button
    con->state[key] = 1;

    // check if it was read
    ck_assert_int_eq(con->state[key], 1);

    // update
    CON_update(con);

    // it should be cleaned up
    ck_assert_int_eq(con->state[key], 0);
    // but not in old_state!
    ck_assert_int_eq(con->old_state[key], 1);

    // update once again!
    CON_update(con);

    // it should be cleaned up
    ck_assert_int_eq(con->state[key], 0);
    // it should be cleaned up
    ck_assert_int_eq(con->old_state[key], 0);

}
END_TEST

START_TEST (CON_button_is_just_pressed_check)
{
    controller_t* con = NULL;
    con               = CON_init();
    Uint8 key         = SDL_SCANCODE_S;
    bool res          = false;
    con->delay        = 0;

    // artifically press one button
    con->state[key] = 1;

    res = CON_button_is_just_pressed(con, key);
    ck_assert_int_eq(res, 1);

    CON_update(con);

    res = CON_button_is_just_pressed(con, key);
    ck_assert_int_eq(res, 0);

    CON_update(con);

    res = CON_button_is_just_pressed(con, key);
    ck_assert_int_eq(res, 0);

}
END_TEST

START_TEST (CON_button_is_just_released_check)
{
    controller_t* con = NULL;
    con               = CON_init();
    Uint8 key         = SDL_SCANCODE_S;
    bool res          = false;
    con->delay        = 0;

    // artifically press one button
    con->state[key] = 1;

    res = CON_button_is_just_released(con, key);
    ck_assert_int_eq(res, 0);

    CON_update(con);

    res = CON_button_is_just_released(con, key);
    ck_assert_int_eq(res, 1);

    CON_update(con);

    res = CON_button_is_just_released(con, key);
    ck_assert_int_eq(res, 0);

}
END_TEST

Suite *controller_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("controller");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, CON_init_check);
    tcase_add_test(tc_core, CON_update_delay_check);
    tcase_add_test(tc_core, CON_update_read_keyboard);
    tcase_add_test(tc_core, CON_button_is_just_pressed_check);
    tcase_add_test(tc_core, CON_button_is_just_released_check);

    suite_add_tcase(s, tc_core);

    return s;
}
