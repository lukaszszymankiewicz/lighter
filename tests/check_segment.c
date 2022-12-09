#include <check.h>
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
    segment_t* head = NULL;
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

START_TEST (SEG_check_determine_type_hor)
{
    // GIVEN 
    int x1 = 1;
    int y1 = 2;
    int x2 = 3;
    int y2 = 2;

    // WHEN
    segment_t* new_segment;
    new_segment = SEG_init(x1, y1, x2, y2);

    // THEN
    ck_assert_int_eq(new_segment->type, HOR);

    // CLEANUP
    SEG_free(new_segment);
}
END_TEST


START_TEST (SEG_check_determine_type_ver)
{
    // GIVEN 
    int x1 = 9;
    int y1 = 1;
    int x2 = 9;
    int y2 = 2;

    // WHEN
    segment_t* new_segment;
    new_segment = SEG_init(x1, y1, x2, y2);

    // THEN
    ck_assert_int_eq(new_segment->type, VER);

    // CLEANUP
    SEG_free(new_segment);
}
END_TEST


START_TEST (SEG_check_determine_type_unknown)
{
    // GIVEN 
    int x1 = 1;
    int y1 = 2;
    int x2 = 3;
    int y2 = 4;

    // WHEN
    segment_t* new_segment;
    new_segment = SEG_init(x1, y1, x2, y2);

    // THEN
    ck_assert_int_eq(new_segment->type, UNKNOWN);

    // CLEANUP
    SEG_free(new_segment);
}
END_TEST

START_TEST (SEG_check_merge_empty_head)
{
    // GIVEN 
    segment_t* head = NULL;
    segment_t* new = NULL;
    segment_t* new2 = NULL;
    segment_t* new3 = NULL;
    segment_t* new4 = NULL;
    segment_t* new5 = NULL;

    new = SEG_init(1,1,1,1);
    new2 = SEG_init(2,2,2,2);

    new3 = SEG_init(3,3,3,3);
    new4 = SEG_init(4,4,4,4);
    new5 = SEG_init(5,5,5,5);

    new4->next = new5;
    new3->next = new4;

    // WHEN
    SEG_merge(&head, new);
    ck_assert_int_eq(SEG_len(head), 1);

    SEG_merge(&head, new2);
    ck_assert_int_eq(SEG_len(head), 2);

    SEG_merge(&head, new3);
    ck_assert_int_eq(SEG_len(head), 5);

    // CLEANUP
    SEG_free(head);
    SEG_free(new);
}
END_TEST


START_TEST (SEG_check_merge_head_has_one_element)
{
    // GIVEN 
    segment_t* head = NULL;
    segment_t* new = NULL;

    segment_t* new2 = NULL;
    segment_t* new3 = NULL;
    segment_t* new4 = NULL;

    head = SEG_init(1,1,1,1);
    new = SEG_init(2,2,2,2);

    new2 = SEG_init(2,2,2,2);
    new3 = SEG_init(3,3,3,3);
    new4 = SEG_init(4,4,4,4);
    
    new3->next=new4;
    new2->next=new3;

    // WHEN
    SEG_merge(&head, new);
    ck_assert_int_eq(SEG_len(head), 2);

    SEG_merge(&head, new2);
    ck_assert_int_eq(SEG_len(head), 5);

    // CLEANUP
    SEG_free(head);
    SEG_free(new);
}
END_TEST


START_TEST (SEG_common_y_check)
{
    // WHEN
    segment_t* first = NULL;
    segment_t* second = NULL;
    int result;

    // THEN
    first = SEG_init(-1, 10, -1, 42);
    second = SEG_init(-1, 10, -1, 69);
    result = SEG_common_y(first, second);
    ck_assert_int_eq(result, 10);

    first = SEG_init(-1, 42, -1, 10);
    second = SEG_init(-1, 69, -1, 10);
    result = SEG_common_y(first, second);
    ck_assert_int_eq(result, 10);

    first = SEG_init(-1, 10, -1, 42);
    second = SEG_init(-1, 69, -1, 10);
    result = SEG_common_y(first, second);
    ck_assert_int_eq(result, 10);

    first = SEG_init(-1, 69, -1, 10);
    second = SEG_init(-1, 10, -1, 42);
    result = SEG_common_y(first, second);
    ck_assert_int_eq(result, 10);

    // CLEANUP
    SEG_free(first);
    SEG_free(second);
}
END_TEST

START_TEST (SEG_common_x_check)
{
    // WHEN
    segment_t* first = NULL;
    segment_t* second = NULL;
    int result;

    // THEN
    first = SEG_init(10, -1, 42, -1);
    second = SEG_init(10, -1, 69, -1);
    result = SEG_common_x(first, second);
    ck_assert_int_eq(result, 10);

    first = SEG_init(42, -1, 10, -1);
    second = SEG_init(69, -1, 10, -1);
    result = SEG_common_x(first, second);
    ck_assert_int_eq(result, 10);

    first = SEG_init(10, -1, 42, -1);
    second = SEG_init(69, -1, 10, -1);
    result = SEG_common_x(first, second);
    ck_assert_int_eq(result, 10);

    first = SEG_init(42, -1, 10, -1);
    second = SEG_init(10, -1, 69, -1);
    result = SEG_common_x(first, second);
    ck_assert_int_eq(result, 10);

    // CLEANUP
    SEG_free(first);
    SEG_free(second);
}
END_TEST

START_TEST (SEG_check_merge_empty_segment)
{
    // GIVEN 
    segment_t* head = NULL;
    segment_t* new = NULL;
    segment_t* new2 = NULL;
    segment_t* new3 = NULL;

    new2 = SEG_init(2,2,2,2);

    // WHEN
    // merging empty segment to empty head
    SEG_merge(&head, new);
    ck_assert_int_eq(SEG_len(head), 0);

    // adding dummy segment to head
    SEG_merge(&head, new2);
    ck_assert_int_eq(SEG_len(head), 1);

    // adding empty segment to non-empty head
    SEG_merge(&head, new3);
    ck_assert_int_eq(SEG_len(head), 1);

    // CLEANUP
    SEG_free(head);
    SEG_free(new);
}
END_TEST


START_TEST (SEG_contains_check)
{
    // GIVEN 
    segment_t* head = NULL;
    segment_t* new = NULL;
    segment_t* new2 = NULL;

    new = SEG_init(1, 2, 3, 4);
    new2 = SEG_init(5, 6, 1, 2);
    bool res;

    // WHEN
    // empty segments
    res = SEG_contains(head, 5,6);
    ck_assert_int_eq(res, 0);

    // adding one segment
    SEG_merge(&head, new);
    res = SEG_contains(head, 5,6);
    ck_assert_int_eq(res, 0);

    res = SEG_contains(head, 1,2);
    ck_assert_int_eq(res, 1);

    res = SEG_contains(head, 3,4);
    ck_assert_int_eq(res, 1);

    // adding second segment
    SEG_merge(&head, new2);

    res = SEG_contains(head, 9, 10);
    ck_assert_int_eq(res, 0);

    res = SEG_contains(head, 1, 2);
    ck_assert_int_eq(res, 1);

    res = SEG_contains(head, 3,4);
    ck_assert_int_eq(res, 0);

    res = SEG_contains(head, 5,6);
    ck_assert_int_eq(res, 0);

    res = SEG_contains(head, 7,8);
    ck_assert_int_eq(res, 0);

}
END_TEST

START_TEST (SEG_closest_to_pt_check)
{
    // GIVEN 
    int x = 0;
    int y = 0;

    segment_t* head = NULL;
    segment_t* new = SEG_init(-10, 10, 10, 10);
    segment_t* new2 = SEG_init(-20, 20, 20, 20);
    segment_t* new3 = SEG_init(-30, 30, 30, 30);

    SEG_merge(&head, new);
    SEG_merge(&head, new2);
    SEG_merge(&head, new3);

    // WHEN
    segment_t* res = SEG_closest_to_pt(head, HOR, x, y);

    // THEN
    ck_assert_int_eq(res->x1, new->x1);
    ck_assert_int_eq(res->y1, new->y1);
    ck_assert_int_eq(res->x2, new->x2);
    ck_assert_int_eq(res->y2, new->y2);
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
    tcase_add_test(tc_core, SEG_check_determine_type_ver);
    tcase_add_test(tc_core, SEG_check_determine_type_hor);
    tcase_add_test(tc_core, SEG_check_determine_type_unknown);
    tcase_add_test(tc_core, SEG_check_merge_empty_head);
    tcase_add_test(tc_core, SEG_check_merge_head_has_one_element);
    tcase_add_test(tc_core, SEG_common_x_check);
    tcase_add_test(tc_core, SEG_common_y_check);
    tcase_add_test(tc_core, SEG_check_merge_empty_segment);
    tcase_add_test(tc_core, SEG_contains_check);
    tcase_add_test(tc_core, SEG_closest_to_pt_check);

    suite_add_tcase(s, tc_core);

    return s;
}
