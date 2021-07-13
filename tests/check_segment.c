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
    ck_assert_int_eq(new_segment->beg.x, 1);
    ck_assert_int_eq(new_segment->beg.y, 2);
    ck_assert_int_eq(new_segment->end.x, 3);
    ck_assert_int_eq(new_segment->end.y, 4);
    ck_assert_ptr_null(new_segment->next);

    // CLEANUP
    SEG_free(new_segment);
}
END_TEST

START_TEST (PT_init_check)
{
    // GIVEN 
    int x = 1; int y = 2;

    // WHEN
    point_t * new_point;
    new_point = PT_init(x, y);

    // THEN
    ck_assert_int_eq(new_point->x, 1);
    ck_assert_int_eq(new_point->y, 2);

    // CLEANUP
    free(new_point);
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

    ck_assert_int_eq(head->beg.x, 1);
    ck_assert_int_eq(head->beg.y, 2);
    ck_assert_int_eq(head->end.x, 3);
    ck_assert_int_eq(head->end.y, 4);

    ck_assert_int_eq(head->next->beg.x, 1);
    ck_assert_int_eq(head->next->beg.y, 2);
    ck_assert_int_eq(head->next->end.x, 3);
    ck_assert_int_eq(head->next->end.y, 4);

    // CLEANUP
    SEG_free(head);
}
END_TEST


START_TEST (SEG_intersects_check)
{
    // GIVEN 
    segment_t * first;
    segment_t * second;
    point_t * intersection_point;

    int x1, x2, x3, x4;
    int y1, y2, y3, y4;
    int intersection;
    intersection_point = PT_init(0, 0);

    // collinear
    x1 = 1; y1 = 5;
    x2 = 3; y2 = 5;
    x3 = 2; y3 = 5;
    x4 = 5; y4 = 5;

    first = SEG_init(x1, y1, x2, y2);
    second = SEG_init(x3, y3, x4, y4);

    intersection = SEG_intersects(*first, *second, intersection_point);
    ck_assert_int_eq(intersection, 0);

    // no collision
    x1 = 1; y1 = 1;
    x2 = 5; y2 = 5;

    x3 = 2; y3 = 2;
    x4 = 8; y4 = 8;

    first = SEG_init(x1, y1, x2, y2);
    second = SEG_init(x3, y3, x4, y4);

    intersection = SEG_intersects(*first, *second, intersection_point);
    ck_assert_int_eq(intersection, 0);

    // collision
    x1 = 3; y1 = 0;
    x2 = 3; y2 = 4;

    x3 = 2; y3 = 3;
    x4 = 4; y4 = 3;

    first = SEG_init(x1, y1, x2, y2);
    second = SEG_init(x3, y3, x4, y4);

    intersection = SEG_intersects(*first, *second, intersection_point);
    ck_assert_int_eq(intersection, 1);
    ck_assert_int_eq(intersection_point->x, 3);
    ck_assert_int_eq(intersection_point->y, 3);

    // collision (edge case)
    x1 = 3; y1 = 0;
    x2 = 3; y2 = 4;

    x3 = 2; y3 = 3;
    x4 = 3; y4 = 4;

    first = SEG_init(x1, y1, x2, y2);
    second = SEG_init(x3, y3, x4, y4);

    intersection = SEG_intersects(*first, *second, intersection_point);
    ck_assert_int_eq(intersection, 1);
    ck_assert_int_eq(intersection_point->x, 3);
    ck_assert_int_eq(intersection_point->y, 4);

    // CLEANUP
    SEG_free(first);
    SEG_free(second);
    free(intersection_point);
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
    tcase_add_test(tc_core, PT_init_check);
    tcase_add_test(tc_core, SEG_PUSH_check);
    tcase_add_test(tc_core, SEG_intersects_check);

    suite_add_tcase(s, tc_core);

    return s;
}
