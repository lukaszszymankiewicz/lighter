#include <check.h>
#include <stdlib.h>
#include "../src/segment.h"


START_TEST (SEG_init_check)
{
    // GIVEN 
    int x1 = 1; int y1 = 2; int x2 = 3; int y2 = 4;

    // WHEN
    segment_t * new_segment;
    new_segment = SEG_init(x1, y1, x2, y2);

    // THEN
    ck_assert_int_eq(new_segment->x1, 1);
    ck_assert_int_eq(new_segment->y1, 2);
    ck_assert_int_eq(new_segment->x2, 3);
    ck_assert_int_eq(new_segment->y2, 4);

    ck_assert_ptr_null(new_segment->next);

    // CLEANUP
    SEG_free(new_segment);
}
END_TEST

START_TEST (SEG_PUSH_check)
{
    // GIVEN 
    int x1 = 1; int y1 = 2; int x2 = 3; int y2 = 4;
    int x3 = 1; int y3 = 2; int x4 = 3; int y4 = 4;
    int len = 0;

    // WHEN
    segment_t * head;
    head = SEG_init(x1, y1, x2, y2);
    SEG_push(&head, x3, y3, x4, y4);

    // THEN
    segment_t * ptr = head;
    for (; ptr; ptr=ptr->next, len++) {}

    ck_assert_int_eq(len, 2);
    ck_assert_ptr_null(head->next->next);

    ck_assert_int_eq(head->x1, 1);
    ck_assert_int_eq(head->y1, 2);
    ck_assert_int_eq(head->x2, 3);
    ck_assert_int_eq(head->y2, 4);

    ck_assert_int_eq(head->next->x1, 1);
    ck_assert_int_eq(head->next->y1, 2);
    ck_assert_int_eq(head->next->x2, 3);
    ck_assert_int_eq(head->next->y2, 4);

    // CLEANUP
    SEG_free(head);
}
END_TEST

Suite * segment_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("segment");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, SEG_init_check);
    tcase_add_test(tc_core, SEG_PUSH_check);

    suite_add_tcase(s, tc_core);

    return s;
}
