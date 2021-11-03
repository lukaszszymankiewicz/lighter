#include <check.h>
#include "../src/obstacle.h"


START_TEST (OBS_len_check) {
    // GIVEN 
    obstacle_t* obstacles = NULL;

    // WHEN && THEN
    ck_assert_int_eq(0,  OBS_len(obstacles));

    OBS_push(&obstacles, 0, 0, 0, 0); // values are just dummies
    ck_assert_int_eq(1,  OBS_len(obstacles));

    OBS_push(&obstacles, 0, 0, 0, 0); // values are just dummies
    ck_assert_int_eq(2,  OBS_len(obstacles));

    OBS_push(&obstacles, 0, 0, 0, 0); // values are just dummies
    ck_assert_int_eq(3,  OBS_len(obstacles));
}
END_TEST

Suite *obstacle_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("obstacle");

    /* Core test case */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, OBS_len_check);
    suite_add_tcase(s, tc_core);

    return s;
}
