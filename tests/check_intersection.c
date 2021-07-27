#include <check.h>
#include "../src/intersection.h"


START_TEST (INTSC_insert_check)
{
    // GIVEN
    x_intersection_t* head;
    INTSC_insert(&head, 2);

    // WHEN
    ck_assert_int_eq(head->x, 2);  // one point inserted, should show "2"

    INTSC_insert(&head, 1);
    ck_assert_int_eq(head->x, 1);  // two points inserted, first one should show "1", second "2"
    ck_assert_int_eq(head->next->x, 2);  

    INTSC_insert(&head, 0);
    ck_assert_int_eq(head->x, 0);  // three points inserted "1", "2" and "3"
    ck_assert_int_eq(head->next->x, 1);  
    ck_assert_int_eq(head->next->next->x, 2);  

    // CLEANUP
    INTSC_free(head);
}
END_TEST

START_TEST (INTSC_get_last_check)
{
    // GIVEN
    x_intersection_t* head = NULL;
    INTSC_insert(&head, 1);
    INTSC_insert(&head, 2);
    INTSC_insert(&head, 0);

    // WHEN
    int last = INTSC_get_last(head);

    // THEN
    ck_assert_int_eq(last, 2);  

    // CLEANUP
    INTSC_free(head);
}
END_TEST

Suite* intersection_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("intersection");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, INTSC_insert_check);
    tcase_add_test(tc_core, INTSC_get_last_check);

    suite_add_tcase(s, tc_core);

    return s;
}
