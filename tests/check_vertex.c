#include <check.h>
#include "../src/vertex.h"

START_TEST (VRTX_eq_check) {
    vertex_t* first = NULL;
    vertex_t* second = NULL;
    bool res;

    // CASE 1
    res = VRTX_eq(first, second);
    ck_assert_int_eq(res, 1);

    // CASE 2
    VRTX_add_point(&first, 1, 1, 0.0);
    res = VRTX_eq(first, second);
    ck_assert_int_eq(res, 0);

    // CASE 3
    VRTX_add_point(&second, 1, 1, 0.0);
    res = VRTX_eq(first, second);
    ck_assert_int_eq(res, 1);

    // CASE 4
    VRTX_add_point(&first, 2, 2, 0.0);
    res = VRTX_eq(first, second);
    ck_assert_int_eq(res, 0);

    // CASE 5
    VRTX_add_point(&second, 2, 2, 0.0);
    res = VRTX_eq(first, second);
    ck_assert_int_eq(res, 1);
}
END_TEST

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


START_TEST (VRTX_transpose_check) {
    // GIVEN
    vertex_t* vertex = NULL;
    vertex           = VRTX_new(100, 100, 0);

    VRTX_add_point(&vertex, 200, 200, 0);

    // WHEN
    VRTX_transpose(vertex, 50, 50);
    
    // THEN
    ck_assert_int_eq(vertex->x, 250);
    ck_assert_int_eq(vertex->y, 250);

    ck_assert_int_eq(vertex->next->x, 150);
    ck_assert_int_eq(vertex->next->y, 150);

}

Suite *vertex_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("vertex");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, VRTX_add_point_check);
    tcase_add_test(tc_core, VRTX_eq_check);
    tcase_add_test(tc_core, VRTX_transpose_check);

    suite_add_tcase(s, tc_core);

    return s;
}
