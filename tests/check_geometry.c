#include <check.h>
#include "../src/geometry.h"


START_TEST (GEO_pt_in_triangle_check_positive)
{
    // GIVEN
    int t1_x = 0; int t1_y = 0;
    int t2_x = 0; int t2_y = 20;
    int t3_x = 20; int t3_y = 0;
    bool result;

    // WHEN && THEN
    result = GEO_pt_in_triangle(5, 5, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 1);

    result = GEO_pt_in_triangle(6, 6, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 1);

    result = GEO_pt_in_triangle(1, 16, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 1);

    result = GEO_pt_in_triangle(16, 1, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 1);

    // THEN
    ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST (GEO_pt_in_triangle_check_edge)
{
    // GIVEN
    int t1_x = 0; int t1_y = 0;
    int t2_x = 0; int t2_y = 20;
    int t3_x = 20; int t3_y = 0;
    bool result;    

    // WHEN && THEN
    result = GEO_pt_in_triangle(0, 0, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 1);

    result = GEO_pt_in_triangle(0, 20, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 1);

    result = GEO_pt_in_triangle(20, 0, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 1);

    result = GEO_pt_in_triangle(10, 10, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST (GEO_pt_in_triangle_check_negative)
{
    // GIVEN
    int t1_x = 0; int t1_y = 0;
    int t2_x = 0; int t2_y = 20;
    int t3_x = 20; int t3_y = 0;

    bool result;
    
    // WHEN && THEN
    result = GEO_pt_in_triangle(40, 40, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 0);

    result = GEO_pt_in_triangle(140, -40, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 0);

    result = GEO_pt_in_triangle(-1, -1, t1_x, t1_y, t2_x, t2_y, t3_x, t3_y);
    ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST (GEO_seg_intersection_with_y_check)
{
    // GIVEN
    int x1 = 10; int y1 = 10;
    int x2 = 20; int y2 = 0;
    float x_inter;
    
    // WHEN && THEN
    x_inter = GEO_seg_intersection_with_y(15, x1, y1, x2, y2);
    ck_assert_float_eq(x_inter, 5.0);

    x_inter = GEO_seg_intersection_with_y(0, x1, y1, x2, y2);
    ck_assert_float_eq(x_inter, 20.0);

    x_inter = GEO_seg_intersection_with_y(10, x1, y1, x2, y2);
    ck_assert_float_eq(x_inter, 10.0);

    x_inter = GEO_seg_intersection_with_y(11, x1, y1, x2, y2);
    ck_assert_float_eq(x_inter, 9.0);
}
END_TEST

START_TEST (GEO_if_seg_intersect_with_y_check_positive)
{
    // GIVEN
    bool result;
    
    // WHEN && THEN
    result = GEO_if_seg_intersect_with_y(1, 0, 10);
    ck_assert_float_eq(result, 1);

    result = GEO_if_seg_intersect_with_y(1, 10, 0);
    ck_assert_float_eq(result, 1);

    result = GEO_if_seg_intersect_with_y(0, 0, 10);
    ck_assert_float_eq(result, 0);

    result = GEO_if_seg_intersect_with_y(0, 10, 0);
    ck_assert_float_eq(result, 0);

    result = GEO_if_seg_intersect_with_y(10, 0, 10);
    ck_assert_float_eq(result, 1);

    result = GEO_if_seg_intersect_with_y(10, 10, 0);
    ck_assert_float_eq(result, 1);

    result = GEO_if_seg_intersect_with_y(11, 0, 10);
    ck_assert_float_eq(result, 0);

    result = GEO_if_seg_intersect_with_y(11, 10, 0);
    ck_assert_float_eq(result, 0);
}
END_TEST

START_TEST (GEO_distance_check)
{
    // GIVEN
    float dist;
    
    // WHEN && THEN
    dist = GEO_distance(0, 10, 10, 10);
    ck_assert_float_eq(dist, 10);

    dist = GEO_distance(10, 10, -10, 10);
    ck_assert_float_eq(dist, 20);

    dist = GEO_distance(10, 10, 20, 20);
    ck_assert_float_ge(dist, 1.41*10);
    ck_assert_float_ge(dist, 1.41*10+0.01);
}
END_TEST

Suite* geometry_suite(void)
{
    Suite* s;
    TCase* tc_core;

    s = suite_create("geometry");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, GEO_pt_in_triangle_check_positive);
    tcase_add_test(tc_core, GEO_pt_in_triangle_check_edge);
    tcase_add_test(tc_core, GEO_pt_in_triangle_check_negative);
    tcase_add_test(tc_core, GEO_seg_intersection_with_y_check);
    tcase_add_test(tc_core, GEO_if_seg_intersect_with_y_check_positive);
    tcase_add_test(tc_core, GEO_distance_check);

    suite_add_tcase(s, tc_core);

    return s;
}
