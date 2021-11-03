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



START_TEST (VRTX_pt_in_segment_check) {
    bool result;

    // FAIL
    result = VRTX_pt_in_segment(10, 10, 15, 10, 20, 20);
    ck_assert_int_eq(result, 0);

    // PASS (vertical)
    result = VRTX_pt_in_segment(10, 10, 15, 10, 20, 10);
    ck_assert_int_eq(result, 1);
    
    // PASS (horizontal)
    result = VRTX_pt_in_segment(10, 10, 10, 15, 10, 20);
    ck_assert_int_eq(result, 1);

    // PASS (vertical edge case)
    result = VRTX_pt_in_segment(10, 10, 15, 10, 15, 10);
    ck_assert_int_eq(result, 1);

    // PASS (horizontal edge case)
    result = VRTX_pt_in_segment(10, 10, 10, 15, 10, 15);
    ck_assert_int_eq(result, 1);
}
END_TEST

 
START_TEST (VRTX_optim_check) {
    vertex_t* head = NULL;
    int st_x = 15;
    int st_y = 15;
    float angle;
    int len;
    int expected_len;

    // base points
    angle = VRTX_calculate_angle(st_x, st_y, 10, 10);
    VRTX_add_point(&head, 10, 10, angle);

    angle = VRTX_calculate_angle(st_x, st_y, 20, 10);
    VRTX_add_point(&head, 20, 10, angle);

    angle = VRTX_calculate_angle(st_x, st_y, 20, 20);
    VRTX_add_point(&head, 20, 20, angle);

    angle = VRTX_calculate_angle(st_x, st_y, 10, 20);
    VRTX_add_point(&head, 10, 20, angle);
    expected_len = VRTX_len(head);    

    //redundant points
    angle = VRTX_calculate_angle(st_x, st_y, 13, 10);
    VRTX_add_point(&head, 13, 10, angle);

    angle = VRTX_calculate_angle(st_x, st_y, 17, 10);
    VRTX_add_point(&head, 17, 10, angle);

    angle = VRTX_calculate_angle(st_x, st_y, 10, 15);
    VRTX_add_point(&head, 10, 15, angle);

    // THEN
    VRTX_optim(head);
    len = VRTX_len(head);
    ck_assert_int_eq(len, expected_len);
}
END_TEST

Suite *vertex_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("vertex");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, VRTX_add_point_check);
    tcase_add_test(tc_core, VRTX_pt_in_segment_check);
    tcase_add_test(tc_core, VRTX_optim_check);

    suite_add_tcase(s, tc_core);

    return s;
}
