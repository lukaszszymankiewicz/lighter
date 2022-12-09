#include <check.h>
#include "../src/geometry.h"
#include "../src/vertex.h"

START_TEST (GEO_seg_intersection_with_y_check)
{
    // GIVEN
    int x1 = 10; int y1 = 10;
    int x2 = 20; int y2 = 0;
    float x_inter;
    
    // WHEN && THEN
    x_inter = GEO_intersection_with_y(15, x1, y1, x2, y2);
    ck_assert_float_eq(x_inter, 5.0);

    x_inter = GEO_intersection_with_y(0, x1, y1, x2, y2);
    ck_assert_float_eq(x_inter, 20.0);

    x_inter = GEO_intersection_with_y(10, x1, y1, x2, y2);
    ck_assert_float_eq(x_inter, 10.0);

    x_inter = GEO_intersection_with_y(11, x1, y1, x2, y2);
    ck_assert_float_eq(x_inter, 9.0);
}
END_TEST

START_TEST (GEO_vertical_segment_intersects_rect_check)
{
    // GIVEN
    int result;

    // SUCCESS
    result = GEO_vertical_segment_intersects_rect(120, 80, 120, 200, 100, 100, 150, 150);
    ck_assert_int_eq(result, 120);

    // edge case
    result = GEO_vertical_segment_intersects_rect(120, 80, 120, 130, 100, 100, 150, 150);
    ck_assert_int_eq(result, 120);

    // edge case
    result = GEO_vertical_segment_intersects_rect(120, 130, 120, 200, 100, 100, 150, 150);
    ck_assert_int_eq(result, 120);

    // edge case
    result = GEO_vertical_segment_intersects_rect(120, 100, 120, 200, 100, 100, 150, 150);
    ck_assert_int_eq(result, 120);

    // FAIL
    result = GEO_vertical_segment_intersects_rect(80, 80, 80, 200, 100, 100, 150, 150);
    ck_assert_int_eq(result, -1);

    result = GEO_vertical_segment_intersects_rect(125, 20, 125, 80, 100, 100, 150, 150);
    ck_assert_int_eq(result, -1);

    result = GEO_vertical_segment_intersects_rect(125, 200, 125, 280, 100, 100, 150, 150);
    ck_assert_int_eq(result, -1);

    result = GEO_vertical_segment_intersects_rect(180, 80, 180, 120, 100, 100, 150, 150);
    ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST (GEO_horizontal_segment_intersects_rect_check)
{
    // GIVEN
    int result;

    // SUCCESS
    result = GEO_horizontal_segment_intersects_rect(80, 120, 200, 120, 100, 100, 150, 150);
    ck_assert_int_eq(result, 120);

    // edge case
    result = GEO_horizontal_segment_intersects_rect(80, 120, 120, 120, 100, 100, 150, 150);
    ck_assert_int_eq(result, 120);

    // edge case
    result = GEO_horizontal_segment_intersects_rect(120, 120, 200, 120, 100, 100, 150, 150);
    ck_assert_int_eq(result, 120);

    // FAIL
    result = GEO_horizontal_segment_intersects_rect(20, 80, 200, 80, 100, 100, 150, 150);
    ck_assert_int_eq(result, -1);

    result = GEO_horizontal_segment_intersects_rect(20, 200, 200, 200, 100, 100, 150, 150);
    ck_assert_int_eq(result, -1);

    result = GEO_horizontal_segment_intersects_rect(20, 125, 80, 125, 100, 100, 150, 150);
    ck_assert_int_eq(result, -1);

    result = GEO_horizontal_segment_intersects_rect(200, 125, 250, 125, 100, 100, 150, 150);
    ck_assert_int_eq(result, -1);
}
END_TEST


START_TEST (GEO_pt_same_side_check)
{
    typedef struct testcase {
          int r_x1;       // beg of ray x
          int r_y1;       // beg of ray y
          int r_x2;       // end of ray x
          int r_y2;       // end of ray y
          int o_x1;       // beg of obstacle x
          int o_y1;       // beg of obstacle y
          int o_x2;       // end of obstacle x
          int o_y2;       // end of obstacle y
          int exp;
    } testcase_t;
    
    int n_cases = 18;

    testcase_t data[] = {
        (testcase_t){40, 30, 40, 70, 20, 50, 50, 50, 0},  
        (testcase_t){30, 10, 50, 10, 20, 50, 50, 50, 1},  
        (testcase_t){30, 0, 50, 20, 20, 50, 50, 50, 1},  
        (testcase_t){0, 70, 100, 40, 20, 50, 50, 50, 1},  
        (testcase_t){100, 40, 0, 70, 20, 50, 50, 50, 1},  

        (testcase_t){30, 0, 100, 0, 20, 50, 50, 50, 1},  
        (testcase_t){30, 0, 30, 50, 20, 50, 50, 50, 0},  
        (testcase_t){30, 0, 20, 50, 20, 50, 50, 50, 0},  
        (testcase_t){20, 50, 30, 0, 20, 50, 50, 50, 0},  
        (testcase_t){20, 50, 30, 100, 20, 50, 50, 50, 0},  

        // collinear
        (testcase_t){30, 50, 40, 50, 10, 50, 20, 50, 0},  
        (testcase_t){30, 50, 40, 50, 10, 50, 35, 50, 0},  
        (testcase_t){30, 50, 40, 50, 35, 50, 50, 50, 0},  
        (testcase_t){30, 50, 40, 50, 60, 50, 90, 50, 0},  

        (testcase_t){50, 30, 50, 80, 50, 10, 50, 20, 0},  
        (testcase_t){50, 30, 50, 80, 50, 10, 50, 40, 0},  
        (testcase_t){50, 30, 50, 80, 50, 40, 50, 90, 0},  
        (testcase_t){50, 30, 50, 80, 50, 90, 50, 95, 0},  
    };

    // WHEN
    for (int i=0; i<n_cases; i++) {
        bool res;
        res = GEO_pt_same_side(
            data[i].r_x1,
            data[i].r_y1,
            data[i].r_x2,
            data[i].r_y2,
            data[i].o_x1,
            data[i].o_y1,
            data[i].o_x2,
            data[i].o_y2
        );

        ck_assert_int_eq(res, data[i].exp);
    }
}
END_TEST


START_TEST ( GEO_collienar_segs_have_common_pt_check)
{
    typedef struct testcase {
          int a1;
          int a2;
          int b1;
          int b2;
          int exp;
    } testcase_t;
    
    int n_cases = 12;

    testcase_t data[] = {
        (testcase_t){10, 20, 30, 40, 0},  
        (testcase_t){40, 30, 20, 10, 0},  
        (testcase_t){0, 0, 20, 10, 0},  
        (testcase_t){10, 30, 20, 40, 1},  
        (testcase_t){10, 30, 20, 0, 1},  

        (testcase_t){10, 30, 30, 40, 1},  
        (testcase_t){10, 30, 15, 20, 1},  
        (testcase_t){5, 20, 10, 40, 1},  
        (testcase_t){20, 50, 10, 40, 1},  
        (testcase_t){0, 10, 10, 40, 1},  

        (testcase_t){20, 30, 10, 40, 1},  
        (testcase_t){20, 50, 80, 90, 0},  
    };

    // WHEN
    for (int i=0; i<n_cases; i++) {
        bool res;
        res = GEO_collienar_segs_have_common_pt(
            data[i].a1,
            data[i].a2,
            data[i].b1,
            data[i].b2
        );

        ck_assert_int_eq(res, data[i].exp);
    }
}
END_TEST

START_TEST (GEO_pt_in_polygon_romboid)
{
    // GIVEN
    vertex_t* vertex = NULL;

    VRTX_add_point(&vertex, 100, 100, 0);
    VRTX_add_point(&vertex, 200, 200, 0);
    VRTX_add_point(&vertex, 300, 100, 0);
    VRTX_add_point(&vertex, 200, 0  , 0);

    typedef struct testcase { int x; int y; int exp; } testcase_t;
    
    testcase_t data[] = {
        // positive
        // (testcase_t){ 100, 100, 1},  
        // (testcase_t){ 200, 200, 1},  
        // (testcase_t){ 300, 100, 1},  
        // (testcase_t){ 200, 0  , 1},  
        // (testcase_t){ 200, 100, 1},  
        // (testcase_t){ 200, 180, 1},  
        // (testcase_t){ 200, 20,  1},  
        // negative
        // (testcase_t){ 80,  100,  0},  
        // (testcase_t){ 200, 210,  0},  
        (testcase_t){ 310, 200,  0},  
        // (testcase_t){ 200, -10,  0},  
    };

    int n_cases = 1;

    for (int i=0; i<n_cases; i++) {
        bool res;

        // WHEN
        res = GEO_pt_in_polygon(vertex, data[i].x, data[i].y);

        // THEN
        ck_assert_float_eq(res, data[i].exp);
    }
}
END_TEST
START_TEST (GEO_pt_in_polygon_rect)
{
    // GIVEN
    vertex_t* vertex = NULL;

    VRTX_add_point(&vertex, 100, 100, 0);
    VRTX_add_point(&vertex, 200, 100, 0);
    VRTX_add_point(&vertex, 200, 200, 0);
    VRTX_add_point(&vertex, 100, 200, 0);

    typedef struct testcase { int x; int y; int exp; } testcase_t;
    
    testcase_t data[] = {
        // positive
        (testcase_t){ 100, 100, 1 },  
        (testcase_t){ 200, 100, 1 },  
        (testcase_t){ 200, 200, 1 },  
        (testcase_t){ 100, 200, 1 },  
        (testcase_t){ 100, 150, 1 },  
        (testcase_t){ 200, 150, 1 },  
        (testcase_t){ 150, 100, 1 },  
        (testcase_t){ 150, 200, 1 },  
        (testcase_t){ 150, 150, 1 },  
        (testcase_t){ 120, 120, 1 },  
        (testcase_t){ 180, 180, 1 },  
        (testcase_t){ 120, 180, 1 },  
        (testcase_t){ 180, 120, 1 },  
        // negative
        (testcase_t){ 50,  150, 0 },  
        (testcase_t){ 250, 150, 0 },  
        (testcase_t){ 150, 50 , 0 },  
        (testcase_t){ 150, 250, 0 },  
        (testcase_t){ 99,  99 , 0 },  
        (testcase_t){ 201, 99 , 0 },  
        (testcase_t){ 201, 201, 0 },  
        (testcase_t){ 99,  201, 0 }  
    };

    int n_cases = 21;

    for (int i=0; i<n_cases; i++) {
        bool res;

        // WHEN
        res = GEO_pt_in_polygon(vertex, data[i].x, data[i].y);

        // THEN
        ck_assert_float_eq(res, data[i].exp);
    }
}
END_TEST

START_TEST (GEO_pt_in_polygon_strange_shape)
{
    // GIVEN
    vertex_t* vertex = NULL;

    VRTX_add_point(&vertex, 100, 100, 0);
    VRTX_add_point(&vertex, 100, 200, 0);
    VRTX_add_point(&vertex, 150, 150, 0);
    VRTX_add_point(&vertex, 170, 200, 0);
    VRTX_add_point(&vertex, 200, 150, 0);
    VRTX_add_point(&vertex, 300, 200, 0);
    VRTX_add_point(&vertex, 350, 150, 0);
    VRTX_add_point(&vertex, 350, 100, 0);

    typedef struct testcase { int x; int y; int exp; } testcase_t;
    
    testcase_t data[] = {
        // positive
        (testcase_t){ 100, 100, 1 },
        (testcase_t){ 100, 200, 1 },
        (testcase_t){ 150, 150, 1 },
        (testcase_t){ 170, 200, 1 },
        (testcase_t){ 200, 150, 1 },
        (testcase_t){ 300, 200, 1 },
        (testcase_t){ 350, 150, 1 },
        (testcase_t){ 350, 100, 1 },
        (testcase_t){ 105, 150, 1 },
        (testcase_t){ 170, 170, 1 },
        (testcase_t){ 300, 180, 1 },
        (testcase_t){ 200, 120, 1 },

        // negative
        (testcase_t){ 110, 200, 0 },
        (testcase_t){ 180, 200, 0 },
        (testcase_t){ 310, 200, 0 },
        (testcase_t){ 360, 180, 0 }
    };

    int n_cases = 16;

    for (int i=0; i<n_cases; i++) {
        bool res;

        // WHEN
        res = GEO_pt_in_polygon(vertex, data[i].x, data[i].y);

        // THEN
        ck_assert_float_eq(res, data[i].exp);
    }
}
END_TEST

Suite* geometry_suite(void)
{
    Suite* s;
    TCase* tc_core;

    s = suite_create("geometry");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, GEO_seg_intersection_with_y_check);
    tcase_add_test(tc_core, GEO_vertical_segment_intersects_rect_check);
    tcase_add_test(tc_core, GEO_horizontal_segment_intersects_rect_check);
    tcase_add_test(tc_core, GEO_pt_same_side_check);
    tcase_add_test(tc_core, GEO_collienar_segs_have_common_pt_check);
    tcase_add_test(tc_core, GEO_pt_in_polygon_rect);
    tcase_add_test(tc_core, GEO_pt_in_polygon_romboid);

    suite_add_tcase(s, tc_core);

    return s;
}
