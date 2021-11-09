#include <check.h>
#include <stdbool.h>
#include "../src/point.h"


START_TEST (PT_eq_check) {
    // GIVEN 
    point_t* first  = PT_new(10, 20);
    point_t* second = PT_new(10, 20);
    bool result;

    // WHEN && THEN
    result = PT_eq(first, second);
    ck_assert_int_eq(result, true);

    first  = PT_new(11, 20);
    second = PT_new(10, 20);

    // WHEN && THEN
    result = PT_eq(first, second);
    ck_assert_int_eq(result, false);
}
END_TEST

START_TEST (PT_push_check) {
    // GIVEN 
    point_t* head = NULL;
    int len;

    len = PT_len(head);
    // no points in linked list
    ck_assert_int_eq(len, 0);
    
    PT_push(&head, 10, 10);
    len = PT_len(head);
    // one point in linked list
    ck_assert_int_eq(len, 1);

    // duplicate point should not enlaege the list
    PT_push(&head, 10, 10);
    len = PT_len(head);
    // one point in linked list
    ck_assert_int_eq(len, 1);

    // adding two new points
    PT_push(&head, 11, 11);
    PT_push(&head, 12, 12);
    len = PT_len(head);
    ck_assert_int_eq(len, 3);

    // adding duplicate which is on middle of the list
    PT_push(&head, 11, 11);
    len = PT_len(head);
    // length should still be equal to 3
    ck_assert_int_eq(len, 3);

    // adding duplicate which is on end of the list
    PT_push(&head, 12, 12);
    len = PT_len(head);
    // length should still be equal to 3
    ck_assert_int_eq(len, 3);

    // adding duplicate which is on beggining of the list
    PT_push(&head, 10, 10);
    len = PT_len(head);
    // length should still be equal to 3
    ck_assert_int_eq(len, 3);

    // adding still works
    PT_push(&head, 13, 13);
    len = PT_len(head);
    // length should still be equal to 4
    ck_assert_int_eq(len, 4);
}
END_TEST


Suite *point_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("point");

    /* Core test case */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, PT_eq_check);
    tcase_add_test(tc_core, PT_push_check);

    suite_add_tcase(s, tc_core);

    return s;
}
