#include <check.h>
#include <stdbool.h>

#include "../src/vertex.h"

START_TEST (VRTX_add_point_check) {
    // GIVEN
    vertex_t* head = NULL;

    // THEN
    VRTX_add_point(&head, 0, 10, 0);
    ck_assert_int_eq(VRTX_len(head), 1);

    VRTX_add_point(&head, 0, 20, 0);
    ck_assert_int_eq(VRTX_len(head), 2);

    VRTX_add_point(&head, 0, 30, 0);
    ck_assert_int_eq(VRTX_len(head), 3);

    VRTX_delete(&head, 25);
    ck_assert_int_eq(VRTX_len(head), 2);

    VRTX_delete(&head, 15);
    ck_assert_int_eq(VRTX_len(head), 1);
}
END_TEST

Suite *vertex_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("vertex");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, VRTX_add_point_check);

    suite_add_tcase(s, tc_core);

    return s;
}
