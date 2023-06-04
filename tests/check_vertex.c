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

START_TEST (VRTX_transpose_check) {
    // GIVEN
    vertex_t* vertex = NULL;
    vertex           = VRTX_new(100, 100, 0);

    VRTX_add_point(&vertex, 200, 200, 0);

    // WHEN
    vertex = VRTX_transpose(vertex, 50, 50);
    
    // THEN
    ck_assert_int_eq(vertex->x, 250);
    ck_assert_int_eq(vertex->y, 250);

    ck_assert_int_eq(vertex->next->x, 150);
    ck_assert_int_eq(vertex->next->y, 150);
}

START_TEST (VRTX_transpose_check_real_life_example) {
    // GIVEN
    int old_len;
    int new_len;

    vertex_t* vertex = NULL;
    vertex           = VRTX_new(475, 438, 0);

    VRTX_add_point(&vertex, 336, 288, 0);
    VRTX_add_point(&vertex, 320, 288, 0);
    VRTX_add_point(&vertex, 320, 448, 0);
    VRTX_add_point(&vertex, 334, 448, 0);

    // WHEN
    old_len = VRTX_len(vertex);
    vertex = VRTX_transpose(vertex, 320, 340);
    new_len = VRTX_len(vertex);
    
    // THEN
    ck_assert_int_eq(old_len, new_len);
}

Suite *vertex_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("vertex");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, VRTX_add_point_check);
    tcase_add_test(tc_core, VRTX_transpose_check);
    tcase_add_test(tc_core, VRTX_transpose_check_real_life_example);

    suite_add_tcase(s, tc_core);

    return s;
}
