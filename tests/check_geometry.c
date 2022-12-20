#include <check.h>
#include "../src/geometry.h"
#include "../src/vertex.h"
#include "../src/segment.h"

START_TEST (GEO_pt_in_rect_check)
{
    bool res;

    res = GEO_pt_in_rect(10, 10, 10, 10, 20, 20);
    ck_assert_int_eq(res, 1);

    res = GEO_pt_in_rect(11, 10, 10, 10, 20, 20);
    ck_assert_int_eq(res, 1);

    res = GEO_pt_in_rect(10, 11, 10, 10, 20, 20);
    ck_assert_int_eq(res, 1);

    res = GEO_pt_in_rect(11, 11, 10, 10, 20, 20);
    ck_assert_int_eq(res, 1);

    res = GEO_pt_in_rect(9, 10, 10, 10, 20, 20);
    ck_assert_int_eq(res, 0);

    res = GEO_pt_in_rect(10, 9, 10, 10, 20, 20);
    ck_assert_int_eq(res, 0);

    res = GEO_pt_in_rect(9, 9, 10, 10, 20, 20);
    ck_assert_int_eq(res, 0);

    res = GEO_pt_in_rect(21, 21, 10, 10, 20, 20);
    ck_assert_int_eq(res, 0);
}
END_TEST

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
        (testcase_t){ 100, 100, 1},  
        (testcase_t){ 200, 200, 1},  
        (testcase_t){ 300, 100, 1},  
        (testcase_t){ 200, 0  , 1},  
        (testcase_t){ 200, 100, 1},  
        (testcase_t){ 200, 180, 1},  
        (testcase_t){ 200, 20,  1},  
        // negative
        (testcase_t){ 80,  100,  0},  
        (testcase_t){ 200, 210,  0},  
        (testcase_t){ 310, 200,  0},  
        (testcase_t){ 200, -10,  0},  
    };

    int n_cases = 11;

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

START_TEST (GEO_polygon_intersecting_rect_polygon_without_any_common_points)
{
    // GIVEN
    segment_t     *rect       = NULL;

    SEG_push(&rect, 0,   0,   0,   100 );
    SEG_push(&rect, 0,   100, 100, 100 );
    SEG_push(&rect, 100, 100, 100, 0   );
    SEG_push(&rect, 100, 0,   0,   0   );

    typedef struct testcase { int x1; int y1; int x2; int y2; int x3; int y3; } testcase_t;
    
    testcase_t data[] = {
        (testcase_t){ -30,    0, -10,  50,  -60,  70 },
        (testcase_t){  50, -100, 200, -10, -100, -20 },
        (testcase_t){ 150,    0, 200,  10,  180,  90 },
        (testcase_t){  50,  120,  70, 200,   10, 210 },
    };

    int n_cases = 4;

    for (int i=0; i<n_cases; i++) {
        vertex_t* res  = NULL;
        vertex_t* poly = NULL;

        // WHEN
        VRTX_add_point(&poly, data[i].x1, data[i].y1, 0);
        VRTX_add_point(&poly, data[i].x2, data[i].y2, 0);
        VRTX_add_point(&poly, data[i].x3, data[i].y3, 0);

        // THEN
        res = GEO_polygon_intersecting_rect(poly, rect, 0, 0);
        ck_assert_ptr_null(res);
    }
}
END_TEST

START_TEST (GEO_polygon_intersecting_rect_polygon_one_common_point)
{
    // GIVEN
    segment_t     *rect       = NULL;

    SEG_push(&rect, 0,   0,   0,   100 );
    SEG_push(&rect, 0,   100, 100, 100 );
    SEG_push(&rect, 100, 100, 100, 0   );
    SEG_push(&rect, 100, 0,   0,   0   );

    typedef struct testcase { int x1; int y1; int x2; int y2; int x3; int y3; } testcase_t;
    
    testcase_t data[] = {
        (testcase_t){   0,   50, -30,   0,  -60,  70 },
        (testcase_t){  50,    0, 200, -10, -100, -20 },
        (testcase_t){ 100,    0, 200,  10,  180,  90 },
        (testcase_t){  50,  100,  70, 200,   10, 210 },

        (testcase_t){   0,    0, -30,   0,  -60,  70 },
        (testcase_t){ 100,    0, 200, -10, -100, -20 },
        (testcase_t){ 100,  100, 200,  10,  180,  90 },
        (testcase_t){   0,  100,  70, 200,   10, 210 },
    };

    int n_cases = 8;

    for (int i=0; i<n_cases; i++) {
        vertex_t* res  = NULL;
        vertex_t* poly = NULL;
        int len;

        // WHEN
        VRTX_add_point(&poly, data[i].x1, data[i].y1, 0);
        VRTX_add_point(&poly, data[i].x2, data[i].y2, 0);
        VRTX_add_point(&poly, data[i].x3, data[i].y3, 0);

        // THEN
        res = GEO_polygon_intersecting_rect(poly, rect, 0, 0);
        len = VRTX_len(res); 
        ck_assert_ptr_nonnull(res);
        ck_assert_int_eq(len, 1);
    }
}
END_TEST

START_TEST (GEO_polygon_intersecting_rect_polygon_two_common_point)
{
    // GIVEN
    segment_t     *rect       = NULL;

    SEG_push(&rect, 0,   0,   0,   100 );
    SEG_push(&rect, 0,   100, 100, 100 );
    SEG_push(&rect, 100, 100, 100, 0   );
    SEG_push(&rect, 100, 0,   0,   0   );

    typedef struct testcase { int x1; int y1; int x2; int y2; int x3; int y3; } testcase_t;
    
    testcase_t data[] = {
        (testcase_t){   0,   10,   0,  90,  -60,  70 },
        (testcase_t){  10,    0, 90,    0,   80, -20 },
        (testcase_t){ 100,   10, 100,  90,  180,  60 },
        (testcase_t){  10,  100,  70, 100,   50, 210 },

        (testcase_t){  50,   50, -30,   0,  -60,  70 },
        (testcase_t){  50,   50, 200, -10, -100, -20 },
        (testcase_t){  50,   50, 200,  10,  180,  90 },
        (testcase_t){  50,   50,  70, 200,   10, 210 },

        (testcase_t){  50,   50,  70,  50,  -60,  70 },
        (testcase_t){  50,   50,  70,  50,   60, -10 },
        (testcase_t){  50,   50,  70,  50,  110,  90 },
        (testcase_t){  50,   50,  70,  50,   60, 110 },
    };

    int n_cases = 12;

    for (int i=0; i<n_cases; i++) {
        vertex_t* res  = NULL;
        vertex_t* poly = NULL;
        int len;

        // WHEN
        VRTX_add_point(&poly, data[i].x1, data[i].y1, 0);
        VRTX_add_point(&poly, data[i].x2, data[i].y2, 0);
        VRTX_add_point(&poly, data[i].x3, data[i].y3, 0);

        // THEN
        res = GEO_polygon_intersecting_rect(poly, rect, 0, 0);
        len = VRTX_len(res); 

        ck_assert_ptr_nonnull(res);
        ck_assert_int_eq(len, 2);
    }
}
END_TEST

START_TEST (GEO_rect_inside_poly_check)
{
    // GIVEN
    segment_t     *rect       = NULL;

    SEG_push(&rect, 0,   0,   0,   100 );
    SEG_push(&rect, 0,   100, 100, 100 );
    SEG_push(&rect, 100, 100, 100, 0   );
    SEG_push(&rect, 100, 0,   0,   0   );

    typedef struct testcase { int x1; int y1; int x2; int y2; int x3; int y3; int n;} testcase_t;
    
    testcase_t data[] = {
        (testcase_t){ -10,   10, -50, -10,  -50,  70, 0 },
        (testcase_t){  90,  -10,  80,   0,   85, -70, 0 },
        (testcase_t){ 110,   90, 180, 200,   85, 170, 0 },
        (testcase_t){ -10,   90,   0, 110,  -85, 170, 0 },

        (testcase_t){  50,   50, -70,  10,    0, -70, 1 },
        (testcase_t){  50,   50, 160,  30,  110, -90, 1 },
        (testcase_t){  50,   50, 200,  60,   80, 140, 1 },
        (testcase_t){  50,   50,  70, 200,  -50, 150, 1 },

        (testcase_t){  50,   50, -10,   0,  -10,  90, 0 },
        (testcase_t){  50,   50,   0, -10,   90, -10, 0 },
        (testcase_t){  50,   50, 110,  10,  110,  90, 0 },
        (testcase_t){  50,   50,  10, 200,   90, 200, 0 },

        (testcase_t){  10,   10, -10, 110,   50, -90, 0 },
        (testcase_t){  10,   10,  90,  10,   50, -90, 0 },
        (testcase_t){  90,   10,  90,  90,  150,  50, 0 },
        (testcase_t){  10,   90,  90,  90,   50, 200, 0 },

        (testcase_t){ -10,   10, 110,  10,   50,-200, 2 },
        (testcase_t){ -10,  110, -10, -10,  150, 110, 3 },
    };

    int n_cases = 18;

    for (int i=0; i<n_cases; i++) {
        vertex_t* res  = NULL;
        vertex_t* poly = NULL;
        int len;

        // WHEN
        VRTX_add_point(&poly, data[i].x1, data[i].y1, 0);
        VRTX_add_point(&poly, data[i].x2, data[i].y2, 0);
        VRTX_add_point(&poly, data[i].x3, data[i].y3, 0);

        // THEN
        res = GEO_rect_inside_poly(poly, rect, 0, 0);
        len = VRTX_len(res); 
        ck_assert_int_eq(len, data[i].n);
    }
}
END_TEST

START_TEST (GEO_vertex_inside_rect_check)
{
    // GIVEN
    typedef struct testcase { int x1; int y1; int x2; int y2; int x3; int y3; int n;} testcase_t;
    
    testcase_t data[] = {
        (testcase_t){ -10,   10, -50, -10,  -50,  70, 0 },
        (testcase_t){  90,  -10,  20,  -5,   85, -70, 0 },
        (testcase_t){ 110,   90, 180, 200,   85, 170, 0 },
        (testcase_t){ -10,   90,   0, 110,  -85, 170, 0 },

        (testcase_t){  50,   50, -70,  10,    0, -70, 1 },
        (testcase_t){  50,   50, 160,  30,  110, -90, 1 },
        (testcase_t){  50,   50, 200,  60,   80, 140, 1 },
        (testcase_t){  50,   50,  70, 200,  -50, 150, 1 },

        (testcase_t){  50,   50, -10,   0,  -10,  90, 1 },
        (testcase_t){  50,   50,   0, -10,   90, -10, 1 },
        (testcase_t){  50,   50, 110,  10,  110,  90, 1 },
        (testcase_t){  50,   50,  10, 200,   90, 200, 1 },

        (testcase_t){ -10,   10, 110,  10,   50,-200, 0 },
        (testcase_t){ -10,  110, -10, -10,  150, 110, 0 },
    };

    int n_cases = 14;

    for (int i=0; i<n_cases; i++) {
        vertex_t* res  = NULL;
        vertex_t* poly = NULL;
        int len;

        // WHEN
        VRTX_add_point(&poly, data[i].x1, data[i].y1, 0);
        VRTX_add_point(&poly, data[i].x2, data[i].y2, 0);
        VRTX_add_point(&poly, data[i].x3, data[i].y3, 0);

        // THEN
        res = GEO_vertex_inside_rect(poly, 0, 0, 100, 100);
        len = VRTX_len(res); 
        ck_assert_int_eq(len, data[i].n);
    }
}
END_TEST

START_TEST (GEO_polygon_union_rect_no_intersection)
{
    // GIVEN
    vertex_t* res  = NULL;
    vertex_t* poly = NULL;
    int x1=0; int y1=0; int x2=100; int y2=100;

    // WHEN
    VRTX_add_point(&poly, -10, -10, 0);
    VRTX_add_point(&poly, -70, -10, 0);
    VRTX_add_point(&poly, -10, -70, 0);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, -1, -1);
    ck_assert_ptr_null(res);
}
END_TEST

START_TEST (GEO_polygon_union_rect_one_point_intersecting_cone_of_rect)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 1;
    int x1=0; int y1=0; int x2=100; int y2=100;

    // WHEN
    VRTX_add_point(&poly,   0,   0, 1);
    VRTX_add_point(&poly, -70, -10, 1);
    VRTX_add_point(&poly, -10, -70, 1);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, -1, -1);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_rect_one_point_intersecting)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;

    int len;
    int expected_len = 1;
    int x1=0; int y1=0; int x2=100; int y2=100;

    // WHEN
    VRTX_add_point(&poly,  10,   0, 1);
    VRTX_add_point(&poly, -70, -10, 1);
    VRTX_add_point(&poly, -10, -70, 1);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, -1, -1);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_rect_two_point_intersecting_simple)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 2;
    int x1=0; int y1=0; int x2=100; int y2=100;

    // WHEN
    VRTX_add_point(&poly,  10,   0, 1);
    VRTX_add_point(&poly,  90,   0, 1);
    VRTX_add_point(&poly,  70, -70, 1);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, -1, -1);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_rect_two_point_intersecting)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 2;
    int x1=0; int y1=0; int x2=100; int y2=100;

    // WHEN
    VRTX_add_point(&poly,   0,  10, 1);
    VRTX_add_point(&poly,   0,  90, 1);
    VRTX_add_point(&poly, -70,  70, 1);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, -1, -1);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);
    
    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_check_three_point_intersection)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 3;
    int x1=0; int y1=0; int x2=100; int y2=100;

    // WHEN
    VRTX_add_point(&poly, -10,  30, 1);
    VRTX_add_point(&poly,  30, -10, 1);
    VRTX_add_point(&poly, -10, -10, 1);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, -1, -1);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_check_four_point_intersection)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 4;
    int x1=0; int y1=0; int x2=100; int y2=100;
    int x0 = 10; int y0 = 10;
    float angle;

    // WHEN
    angle = GEO_angle_2pt(x0, y0, -10, 30);
    VRTX_add_point(&poly, -10,  30, angle);

    angle = GEO_angle_2pt(x0, y0, 30, -10);
    VRTX_add_point(&poly, 30, -10, angle);

    angle = GEO_angle_2pt(x0, y0, -10, -10);
    VRTX_add_point(&poly, -10, -10, angle);

    angle = GEO_angle_2pt(x0, y0, 30, 30);
    VRTX_add_point(&poly, 30, 30, angle);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, x0, y0);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_check_real_life_example2)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 5;
    int x1=0; int y1=0; int x2=SCREEN_WIDTH; int y2=SCREEN_HEIGHT;
    int x0 = 155; int y0 = 98;
    float angle;

    // WHEN
    angle = GEO_angle_2pt(x0, y0, 155, 98);
    VRTX_add_point(&poly, 155, 98, angle);

    angle = GEO_angle_2pt(x0, y0, 16, -52);
    VRTX_add_point(&poly, 16, -52, angle);

    angle = GEO_angle_2pt(x0, y0, 0, -52);
    VRTX_add_point(&poly, 0, -52, angle);

    angle = GEO_angle_2pt(x0, y0, 0, 108);
    VRTX_add_point(&poly, 0, 108, angle);

    angle = GEO_angle_2pt(x0, y0, 14, 108);
    VRTX_add_point(&poly, 14, 108, angle);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, x0, y0);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_check_real_life_example)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 4;
    int x1=0; int y1=0; int x2=SCREEN_WIDTH; int y2=SCREEN_HEIGHT;
    int x0 = 155; int y0 = 98;
    float angle;

    // WHEN
    angle = GEO_angle_2pt(x0, y0, 155, 98);
    VRTX_add_point(&poly, 155, 98, angle);

    angle = GEO_angle_2pt(x0, y0, 64, -9);
    VRTX_add_point(&poly, 64, -9, angle);

    angle = GEO_angle_2pt(x0, y0, 64, 104);
    VRTX_add_point(&poly, 64, 104, angle);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, x0, y0);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_check_two_point_intersection_collinear)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 2;
    int x1=0; int y1=0; int x2=100; int y2=100;

    // WHEN
    VRTX_add_point(&poly, -10,   0, 1);
    VRTX_add_point(&poly,  30,   0, 1);
    VRTX_add_point(&poly,  30, -10, 1);
    VRTX_add_point(&poly, -10, -10, 1);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, -1, -1);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

START_TEST (GEO_polygon_union_check_typical_light_shape)
{
    // GIVEN
    vertex_t* res    = NULL;
    vertex_t* poly   = NULL;
    int len;
    int expected_len = 4;
    int x1=0; int y1=0; int x2=100; int y2=100;
    int y0 = 50; int x0 = 50;
    float angle;

    // WHEN
    angle = GEO_angle_2pt(x0, y0, 50, 50);
    VRTX_add_point(&poly,  50,  50, angle);

    angle = GEO_angle_2pt(x0, y0, 90, 0);
    VRTX_add_point(&poly, 90, 0, angle);

    angle = GEO_angle_2pt(x0, y0, 0, 90);
    VRTX_add_point(&poly, 0, 90, angle);

    angle = GEO_angle_2pt(x0, y0, -10, -10);
    VRTX_add_point(&poly, -10, -10, angle);

    // THEN
    res = GEO_polygon_union_rect(poly, x1, y1, x2, y2, x0, y0);
    len = VRTX_len(res); 
    ck_assert_ptr_nonnull(res);
    ck_assert_int_eq(len, expected_len);

    vertex_t* ptr = NULL;
    ptr           = res;

    while(ptr) {
        bool isin;
        isin = GEO_pt_in_rect(ptr->x, ptr->y, x1, y1, x2, y2);
        ck_assert_int_eq(isin, 1);
        ptr=ptr->next;
    }
}
END_TEST

Suite* geometry_suite(void)
{
    Suite* s;
    TCase* tc_core;

    s = suite_create("geometry");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, GEO_pt_in_rect_check);
    tcase_add_test(tc_core, GEO_seg_intersection_with_y_check);
    tcase_add_test(tc_core, GEO_vertical_segment_intersects_rect_check);
    tcase_add_test(tc_core, GEO_horizontal_segment_intersects_rect_check);
    tcase_add_test(tc_core, GEO_pt_same_side_check);
    tcase_add_test(tc_core, GEO_collienar_segs_have_common_pt_check);
    tcase_add_test(tc_core, GEO_pt_in_polygon_rect);
    tcase_add_test(tc_core, GEO_pt_in_polygon_romboid);
    tcase_add_test(tc_core, GEO_pt_in_polygon_strange_shape);
    tcase_add_test(tc_core, GEO_polygon_intersecting_rect_polygon_without_any_common_points);
    tcase_add_test(tc_core, GEO_polygon_intersecting_rect_polygon_one_common_point);
    tcase_add_test(tc_core, GEO_polygon_intersecting_rect_polygon_two_common_point);
    tcase_add_test(tc_core, GEO_rect_inside_poly_check);
    tcase_add_test(tc_core, GEO_vertex_inside_rect_check);
    tcase_add_test(tc_core, GEO_polygon_union_rect_one_point_intersecting_cone_of_rect);
    tcase_add_test(tc_core, GEO_polygon_union_rect_no_intersection);
    tcase_add_test(tc_core, GEO_polygon_union_rect_one_point_intersecting);
    tcase_add_test(tc_core, GEO_polygon_union_rect_two_point_intersecting);
    tcase_add_test(tc_core, GEO_polygon_union_rect_two_point_intersecting_simple);
    tcase_add_test(tc_core, GEO_polygon_union_check_three_point_intersection);
    tcase_add_test(tc_core, GEO_polygon_union_check_two_point_intersection_collinear);
    tcase_add_test(tc_core, GEO_polygon_union_check_typical_light_shape);
    tcase_add_test(tc_core, GEO_polygon_union_check_four_point_intersection);
    tcase_add_test(tc_core, GEO_polygon_union_check_real_life_example);
    tcase_add_test(tc_core, GEO_polygon_union_check_real_life_example2);

    suite_add_tcase(s, tc_core);

    return s;
}
