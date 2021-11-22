#include <check.h>
#include "../src/sorted_list.h"


START_TEST (SRTLST_insert_check)
{
    // GIVEN
    sorted_list_t* head;
    SRTLST_insert(&head, 2);

    // WHEN
    ck_assert_int_eq(head->value, 2);  // one point inserted, should show "2"

    SRTLST_insert(&head, 1);
    ck_assert_int_eq(head->value, 1);  // two points inserted, first one should show "1", second "2"
    ck_assert_int_eq(head->next->value, 2);  

    SRTLST_insert(&head, 0);
    ck_assert_int_eq(head->value, 0);  // three points inserted "1", "2" and "3"
    ck_assert_int_eq(head->next->value, 1);  
    ck_assert_int_eq(head->next->next->value, 2);  

    // CLEANUP
    SRTLST_free(head);
}
END_TEST

START_TEST (SRTLST_get_last_check)
{
    // GIVEN
    sorted_list_t* head = NULL;
    SRTLST_insert(&head, 1);
    SRTLST_insert(&head, 2);
    SRTLST_insert(&head, 0);

    // WHEN
    int last = SRTLST_get_last(head);

    // THEN
    ck_assert_int_eq(last, 2);  

    // CLEANUP
    SRTLST_free(head);
}
END_TEST

Suite* sorted_list_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("sorted_list");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, SRTLST_insert_check);
    tcase_add_test(tc_core, SRTLST_get_last_check);

    suite_add_tcase(s, tc_core);

    return s;
}
