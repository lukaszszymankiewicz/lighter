#include <check.h>
#include <stdlib.h>
#include "../src/lightpt.h"

START_TEST (LIGPT_calculate_angle_check)
{
    // GIVEN
    enum bound {LOWER_BOUND, UPPER_BOUND};   // just fo the sake of readability
    enum coord {X1, Y1, X2, Y2};
    int n_cases = 4;
    
    int cases[4][4] = {
        {100, 100, 100, 200},  // 180 degrees
        {200, 100, 100, 100},  // 90 degrees
        {100, 100, 100, 0},    // 0 degrees
        {0, 100, 100, 100}     // -90 degrees
    };

    // lower and upper bound
    float expected_angles[4][2] = {
        {3.14150, 3.14160},
        {1.57070, 1.57090},
        {0.00000, 0.00000},
        {-1.57080, -1.57070}
    };


    // WHEN
    for (int i=0; i<n_cases; i++)
    {
        // THEN
        float angle = LIGPT_calculate_angle(cases[i][X1], cases[i][Y1], cases[i][X2], cases[i][Y2]);
        ck_assert_float_ge(angle, expected_angles[i][LOWER_BOUND]);
        ck_assert_float_le(angle, expected_angles[i][UPPER_BOUND]);
    }


}
END_TEST

START_TEST (LIGPT_new_check)
{
    // GIVEN
    int x = 100;
    int y = 200;
    float angle = 3.1415;
    int wall_id = 5;
    lightpoint_t* new_point = NULL;

    // WHEN
    new_point = LIGPT_new(x, y, angle, wall_id);

    // THEN
    ck_assert_int_eq(new_point->x, 100);
    ck_assert_int_eq(new_point->y, 200);
    ck_assert_int_eq(new_point->angle, 3.1415);
    ck_assert_int_eq(new_point->wall_id, 5);
    ck_assert_ptr_null(new_point->next);

    // CLEANUP
    LIGPT_free(new_point);
}
END_TEST

START_TEST (LIGPT_insert_check)
{
    // GIVEN
    lightpoint_t* head = NULL;

    // this point should be second
    int x1 = 1;
    int y1 = 1;
    float angle1 = 2;
    int wall_id1 = 2;

    // this point should be third
    int x2 = 1;
    int y2 = 1;
    float angle2 = 3;
    int wall_id2 = 3;

    // this point should be first
    int x3 = 1;
    int y3 = 1;
    float angle3 = 1;
    int wall_id3 = 1;

    // WHEN
    LIGPT_insert(&head, x1, y1, angle1, wall_id1);
    LIGPT_insert(&head, x2, y2, angle2, wall_id2);
    LIGPT_insert(&head, x3, y3, angle3, wall_id3);

    // THEN
    // yeah, we will get points by its wall ids.
    ck_assert_int_eq(head->wall_id, 1);
    ck_assert_int_eq(head->next->wall_id, 2);
    ck_assert_int_eq(head->next->next->wall_id, 3);
    ck_assert_ptr_null(head->next->next->next);

    // CLEANUP
    LIGPT_free(head);
}
END_TEST


START_TEST (LIGPT_optim_check)
{
    // GIVEN
    lightpoint_t* poly = NULL;
    int expected_len = 9;
    int expected_ids[9] = {5, 6, 6, 7, 8, 8, 9, 9, 10};

    // lets populate polygon with some point, coords are irrelevant
    // angle is set in a way that such points wont be sorted
    LIGPT_insert(&poly, 1, 1, 1, 5);
    LIGPT_insert(&poly, 1, 1, 2, 6);
    LIGPT_insert(&poly, 1, 1, 2, 6);  // this point is redundant
    LIGPT_insert(&poly, 1, 1, 2, 6);
    LIGPT_insert(&poly, 1, 1, 3, 7);
    LIGPT_insert(&poly, 1, 1, 4, 8);
    LIGPT_insert(&poly, 1, 1, 4, 8);  // this point is redundant
    LIGPT_insert(&poly, 1, 1, 4, 8);
    LIGPT_insert(&poly, 1, 1, 5, 9);
    LIGPT_insert(&poly, 1, 1, 5, 9);  // this point is redundant
    LIGPT_insert(&poly, 1, 1, 5, 9);
    LIGPT_insert(&poly, 1, 1, 6, 10);

    // WHEN
    LIGPT_optim(poly);

    // THEN
    lightpoint_t* ptr = NULL;
    ptr = poly;

    for (int i=0; i<expected_len; i++)
    {
        ck_assert_int_eq(ptr->wall_id, expected_ids[i]);
        ptr = ptr->next;
    }
    // CLEANUP
    LIGPT_free(poly);
}
END_TEST

Suite * lightpt_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("lightpoint");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, LIGPT_calculate_angle_check);
    tcase_add_test(tc_core, LIGPT_new_check);
    tcase_add_test(tc_core, LIGPT_insert_check);
    tcase_add_test(tc_core, LIGPT_optim_check);

    suite_add_tcase(s, tc_core);

    return s;
}
