#include <check.h>
#include <stdlib.h>
#include <math.h>
#include "../src/light.h"
#include "../src/segment.h"
#include "../src/vertex.h"
#include "../src/geometry.h"
#include "../src/point.h"
#include "../src/global.h"

// this test check only base_light_polygon, but to be absolutely sure its result is check against
// full polygon check (for width equal to 0.0).
START_TEST (LIG_get_base_light_polygon_check) 
{
    // GIVEN
    segment_t *obstacles         = NULL;
    vertex_t  *light_vertex      = NULL;
    vertex_t  *full_light_vertex = NULL;
    point_t   *hit_points        = NULL;

    bool eq;

    int calculated_length;
    int x; 
    int y; 
    float angle = 0.0;  // irrelevant
    float width = 0.0;  // only for sanity checks

    // CASE 1 (only border)
    x = 50; 
    y = 50; 
    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    hit_points = LIG_generate_hit_points(x, y, 0.0, angle, obstacles);
    light_vertex = LIG_get_base_light_polygon(x, y, obstacles, hit_points);
    calculated_length = VRTX_len(light_vertex);
    ck_assert_int_eq(calculated_length, 4);
    full_light_vertex = LIG_get_light_polygon(x, y, width, angle, obstacles);
    eq = VRTX_eq(light_vertex, full_light_vertex);
    ck_assert_int_eq(eq, 1);

    light_vertex      = NULL;
    full_light_vertex = NULL;
    obstacles         = NULL;
    hit_points        = NULL;

    // CASE 2 (border and one rect obstacle)
    // border
    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    // obstacles
    SEG_push(&obstacles, 10, 10, 20, 10);
    SEG_push(&obstacles, 20, 10, 20, 50);
    SEG_push(&obstacles, 20, 50, 10, 50);
    SEG_push(&obstacles, 10, 50, 10, 10);

    // THEN
    x = 80;
    y = 90;
    hit_points = LIG_generate_hit_points(x, y, width, angle, obstacles);
    light_vertex = LIG_get_base_light_polygon(x, y, obstacles, hit_points);
    calculated_length = VRTX_len(light_vertex);
    ck_assert_int_eq(calculated_length, 8);
    full_light_vertex = LIG_get_light_polygon(x, y, width, angle, obstacles);
    eq = VRTX_eq(light_vertex, full_light_vertex);

    ck_assert_int_eq(eq, 1);

    light_vertex      = NULL;
    full_light_vertex = NULL;
    obstacles         = NULL;
    hit_points        = NULL;

    // CASE 3 (border and one rect obstacle)
    // border
    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    // obstacles
    SEG_push(&obstacles, 90, 90, 90, 60);
    SEG_push(&obstacles, 90, 60, 80, 60);
    SEG_push(&obstacles, 80, 60, 80, 90);
    SEG_push(&obstacles, 80, 90, 90, 90);

    // THEN
    x = 10;
    y = 30;
    hit_points = LIG_generate_hit_points(x, y, width, angle, obstacles);
    light_vertex = LIG_get_base_light_polygon(x, y, obstacles, hit_points);
    calculated_length = VRTX_len(light_vertex);
    ck_assert_int_eq(calculated_length, 8);
    full_light_vertex = LIG_get_light_polygon(x, y, width, angle, obstacles);
    eq = VRTX_eq(light_vertex, full_light_vertex);


    light_vertex      = NULL;
    full_light_vertex = NULL;
    obstacles         = NULL;
    hit_points        = NULL;

    // CASE 4 (border and two rect obstacle)
    // border
    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    // first square obstacle
    SEG_push(&obstacles, 10, 10, 20, 10);
    SEG_push(&obstacles, 20, 10, 20, 20);
    SEG_push(&obstacles, 20, 20, 10, 20);
    SEG_push(&obstacles, 10, 20, 10, 10);

    // second square obstacle
    SEG_push(&obstacles, 80, 80, 90, 80);
    SEG_push(&obstacles, 90, 80, 90, 90);
    SEG_push(&obstacles, 90, 90, 80, 90);
    SEG_push(&obstacles, 80, 90, 80, 80);

    // THEN
    x = 10;
    y = 80;
    hit_points = LIG_generate_hit_points(x, y, 0.0, angle, obstacles);
    light_vertex = LIG_get_base_light_polygon(x, y, obstacles, hit_points);
    calculated_length = VRTX_len(light_vertex);
    ck_assert_int_eq(calculated_length, 12);
    full_light_vertex = LIG_get_light_polygon(x, y, width, angle, obstacles);
    eq = VRTX_eq(light_vertex, full_light_vertex);
    ck_assert_int_eq(eq, 1);

    full_light_vertex = NULL;
    light_vertex      = NULL;
    obstacles         = NULL;
    hit_points        = NULL;

    // CASE 5 (two borders)
    // border
    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    // second border
    SEG_push(&obstacles, 20, 20, 80, 20);  //upper
    SEG_push(&obstacles, 20, 80, 80, 80);  //donwer
    SEG_push(&obstacles, 20, 20, 20, 80);  //left
    SEG_push(&obstacles, 80, 20, 80, 80);  //right

    // THEN
    x = 50;
    y = 50;
    hit_points = LIG_generate_hit_points(x, y, width, angle, obstacles);
    light_vertex = LIG_get_base_light_polygon(x, x, obstacles, hit_points);
    calculated_length = VRTX_len(light_vertex);
    ck_assert_int_eq(calculated_length, 4);
    full_light_vertex = LIG_get_light_polygon(x, y, width, angle, obstacles);
    eq = VRTX_eq(light_vertex, full_light_vertex);
    ck_assert_int_eq(eq, 1);

    full_light_vertex = NULL;
    light_vertex      = NULL;
    obstacles         = NULL;
    hit_points        = NULL;

    // CASE 6 (border and rect obstacle in front)
    // border
    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    // first square obstacle
    SEG_push(&obstacles, 20, 10, 10, 10);
    SEG_push(&obstacles, 20, 20, 20, 10);
    SEG_push(&obstacles, 20, 20, 10, 20);
    SEG_push(&obstacles, 10, 20, 10, 10);

    // THEN
    x = 10;
    y = 30;
    hit_points = LIG_generate_hit_points(x, y, width, angle, obstacles);
    light_vertex = LIG_get_base_light_polygon(x, y, obstacles, hit_points);
    calculated_length = VRTX_len(light_vertex);
    ck_assert_int_eq(calculated_length, 8);
    full_light_vertex = LIG_get_light_polygon(x, y, width, angle, obstacles);
    eq = VRTX_eq(light_vertex, full_light_vertex);
    ck_assert_int_eq(eq, 1);
}
END_TEST

START_TEST (LIG_get_light_polygon_check) 
{
    // GIVEN
    segment_t *obstacles         = NULL;
    vertex_t  *light_vertex      = NULL;

    int calculated_length;
    int x1, x2; int y1, y2;
    float angle; float width;

    // CASE 1 (only border)
    x1 = 50; y1 = 50; 
    x2 = 0; y2 = 50; 

    angle = GEO_angle_2pt(x1, y1, x2, y2);
    width = PI/3;

    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    light_vertex = LIG_get_light_polygon(x1, y1, width, angle, obstacles);
    calculated_length = VRTX_len(light_vertex);

    ck_assert_int_eq(calculated_length, 2+2+1);
    obstacles = NULL; light_vertex = NULL;

    // CASE 2 (border + one obstacle in sight)
    x1 = 50; y1 = 50; 
    x2 = 0; y2 = 50; 

    angle = GEO_angle_2pt(x1, y1, x2, y2);
    width = PI/3;

    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    SEG_push(&obstacles, 10, 10, 20, 10);
    SEG_push(&obstacles, 20, 10, 20, 20);
    SEG_push(&obstacles, 20, 20, 10, 20);
    SEG_push(&obstacles, 10, 20, 10, 10);

    light_vertex = LIG_get_light_polygon(x1, y1, width, angle, obstacles);
    calculated_length = VRTX_len(light_vertex);

    ck_assert_int_eq(calculated_length, 1+2+4+2);
    obstacles = NULL; light_vertex = NULL;

    // CASE 2 (border + one obstacle in sight + one obstacle not in sight)
    x1 = 50; y1 = 50; 
    x2 = 0; y2 = 50; 

    angle = GEO_angle_2pt(x1, y1, x2, y2);
    width = PI/3;

    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    SEG_push(&obstacles, 10, 10, 20, 10);
    SEG_push(&obstacles, 20, 10, 20, 20);
    SEG_push(&obstacles, 20, 20, 10, 20);
    SEG_push(&obstacles, 10, 20, 10, 10);

    SEG_push(&obstacles, 80, 80, 90, 80);
    SEG_push(&obstacles, 90, 80, 90, 90);
    SEG_push(&obstacles, 90, 90, 80, 90);
    SEG_push(&obstacles, 80, 90, 80, 80);

    light_vertex = LIG_get_light_polygon(x1, y1, width, angle, obstacles);
    calculated_length = VRTX_len(light_vertex);
    ck_assert_int_eq(calculated_length, 1+2+4+2);

    obstacles = NULL; light_vertex = NULL;

    // CASE 3 (taken from game itself)
    x1 = 165; y1 = 155;

    angle = 4.757250; width = 0.448786;

    SEG_push(&obstacles, 0, 0, 320, 0);
    SEG_push(&obstacles, 0, 200, 320, 200);
    SEG_push(&obstacles, 0, 0, 0, 200);
    SEG_push(&obstacles, 320, 0, 320, 200);
    SEG_push(&obstacles, 33, 0, 33, 196);
    SEG_push(&obstacles, 33, 0, 289, 0);
    SEG_push(&obstacles, 65, 4, 65, 164);
    SEG_push(&obstacles, 33, 196, 289, 196);
    SEG_push(&obstacles, 65, 4, 257, 4);
    SEG_push(&obstacles, 65, 164, 257, 164);
    SEG_push(&obstacles, 289, 0, 289, 196);
    SEG_push(&obstacles, 257, 4, 257, 164);

    light_vertex = LIG_get_light_polygon(x1, y1, width, angle, obstacles);
    calculated_length = VRTX_len(light_vertex);
    ck_assert_int_eq(calculated_length, 4);

    obstacles = NULL; light_vertex = NULL;

}
END_TEST

START_TEST (LIG_ray_intersects_multiple_check_border_case) 
{
    // GIVEN
    segment_t* obs   = NULL;
    segment_t* inter = NULL;

    SEG_push(&obs, 30, 80, 30, 20);
    SEG_push(&obs, 50, 0, 50, 100);

    inter = LIG_ray_intersects_multiple(0, 80, 100, 80, obs);

    ck_assert_int_eq(SEG_len(inter), 2);
}
END_TEST

START_TEST (LIG_ray_intersects_multiple_check_hit_into_corner) 
{
    // GIVEN
    segment_t* obs   = NULL;
    segment_t* inter = NULL;

    SEG_push(&obs, 0, 50, 50, 50);
    SEG_push(&obs, 50, 50, 50, 0);

    inter = LIG_ray_intersects_multiple(0,0,100,100,obs);

    ck_assert_int_eq(SEG_len(inter), 2);
}
END_TEST

START_TEST (LIG_ray_intersects_multiple_check_hit_into_reversed_corner) 
{
    // GIVEN
    segment_t* obs   = NULL;
    segment_t* inter = NULL;

    SEG_push(&obs, 0, 50, 50, 50);
    SEG_push(&obs, 50, 50, 50, 100);

    inter = LIG_ray_intersects_multiple(0, 100, 100, 0, obs);

    ck_assert_int_eq(SEG_len(inter), 2);
}
END_TEST

START_TEST (LIG_ray_intersects_multiple_check_hit_collinear_obstacles) 
{
    // GIVEN
    int x = 10;
    int y = 40;

    int x2 = 10;
    int y2 = 20;

    segment_t* obs   = NULL;
    segment_t* inter = NULL;

    SEG_push(&obs, 10, 20, 20, 20);
    SEG_push(&obs, 10, 20, 10, 10);
    SEG_push(&obs, 20, 20, 20, 10);
    SEG_push(&obs, 20, 10, 10, 10);

    inter = LIG_ray_intersects_multiple(x, y, x2, y2, obs);

    ck_assert_int_eq(SEG_len(inter), 2);
}
END_TEST

START_TEST (LIG_ray_intersects_multiple_check_multiple_obstacles) 
{
    // GIVEN
    segment_t* obs   = NULL;
    segment_t* inter = NULL;

    SEG_push(&obs, 20, 110, 20, 0);
    SEG_push(&obs, 50, 50, 90, 50);
    SEG_push(&obs, 120, 0, 90, 0);
    SEG_push(&obs, -10, -10, -10, -20);
    SEG_push(&obs, 0, 10, 0, 90);

    SEG_push(&obs, 10, 100, 100, 100);

    inter = LIG_ray_intersects_multiple(0, 100, 100, 0, obs);

    segment_t* ptr = NULL;
    ptr= inter;

    while(ptr) {
        ptr=ptr->next;
    }

    ck_assert_int_eq(SEG_len(inter), 3);
}
END_TEST

START_TEST (LIG_calculate_dull_ray_end_check) 
{

    typedef struct testcase {
          int r_beg;
          int r_end;
          int o_beg;
          int o_end;
          int exp_len; // expected number of spliover segments
    } testcase_t;
    
    int n_cases = 5;

    testcase_t data[] = {
        (testcase_t){50, 100, 30, 80, 50},
        (testcase_t){50, 100, 50, 80, 50},
        (testcase_t){50, 100, 80, 120, 80},
        (testcase_t){50, 100, 90, 120, 90},
        (testcase_t){100, 50, 30, 60, 60},
        (testcase_t){100, 50, 30, 80, 80},
    };

    for (int i=0; i<n_cases; i++) {
        int res = LIG_calculate_dull_ray_end(
            data[i].r_beg,
            data[i].r_end,
            data[i].o_beg,
            data[i].o_end
        );

        ck_assert_int_eq(res, data[i].exp_len);
    };
}
END_TEST

// in this test rays is casted on circle radius and check if auxilary ray ends up behind the screen
// border (this is expected behavior!)
START_TEST (LIG_generate_slipover_hit_point_check) 
{
    // GIVEN
    int x1 = 100;
    int y1 = 100;
    int r = 50; 
    point_t* pt = NULL;

    for (int ang=0; ang<360; ang++) {
        // point on circle
        int x2 = (int)(r * cos(ang) + x1);
        int y2 = (int)(r * sin(ang) + y1);

        pt = LIG_generate_slipover_hit_point(x1, y1, x2, y2);

        bool x_outside = pt->x<=0 || pt->x >= SCREEN_WIDTH;
        bool y_outside = pt->y<=0 || pt->y >= SCREEN_HEIGHT;
        
        ck_assert((x_outside || y_outside) == true);
    }
}
END_TEST

START_TEST (LIG_generate_aux_hit_points_edge_case)
{
    // CASE 1
    int x = 10;
    int y = 40;
    int x2 = 10;
    int y2 = 20;

    segment_t* obs   = NULL;
    point_t* aux_pt  = NULL;

    SEG_push(&obs, 10, 20, 20, 20);
    SEG_push(&obs, 10, 20, 10, 10);
    aux_pt = LIG_generate_aux_hit_points(x, y, x2, y2, obs);
    ck_assert_ptr_nonnull(aux_pt);

    // CASE 2
    x = 40;
    y = 20;
    x2 = 20;
    y2 = 20;

    obs   = NULL;
    aux_pt  = NULL;

    SEG_push(&obs, 10, 20, 20, 20);
    SEG_push(&obs, 10, 20, 10, 10);
    aux_pt = LIG_generate_aux_hit_points(x, y, x2, y2, obs);
    ck_assert_ptr_nonnull(aux_pt);

    // CASE 3
    x = 1;
    y = 20;
    x2 = 5;
    y2 = 20;

    obs   = NULL;
    aux_pt  = NULL;

    SEG_push(&obs, 10, 20, 20, 20);
    SEG_push(&obs, 10, 20, 10, 10);
    aux_pt = LIG_generate_aux_hit_points(x, y, x2, y2, obs);
    ck_assert_ptr_nonnull(aux_pt);

    // CASE 3
    x = 10;
    y = 1;
    x2 = 10;
    y2 = 10;

    obs   = NULL;
    aux_pt  = NULL;

    SEG_push(&obs, 10, 20, 20, 20);
    SEG_push(&obs, 10, 20, 10, 10);
    aux_pt = LIG_generate_aux_hit_points(x, y, x2, y2, obs);
    ck_assert_ptr_nonnull(aux_pt);
}
END_TEST

START_TEST (LIG_generate_aux_hit_points_check_ray_inside_corner_aux_pt_generated) 
{
    // GIVEN
    int x1 = 100;
    int y1 = 100;

    segment_t* corner = NULL;
    segment_t* seg_a  = NULL;
    segment_t* seg_b  = NULL;
    point_t* aux      = NULL;

    int len = 1;
    int len2 = 50;

    // WHEN && THEN
    typedef struct testcase {
          int x;       // end of ray x
          int y;       // end of ray y
          int wid;     // corner width
          int hei;     // corner height
          int exp_seg; // expected number of spliover segments
    } testcase_t;
    
    int n_cases = 64;

    testcase_t data[] = {
        // upper-left corner
        // convex
        (testcase_t){80, 80, -len, -len, 0},
        (testcase_t){80, 80, -len, -len2, 0},
        (testcase_t){80, 80, -len2, -len, 0},
        (testcase_t){80, 80, -len2, -len2, 0},

        // concave
        (testcase_t){80, 80, len, len, 0},
        (testcase_t){80, 80, len, len2, 0},
        (testcase_t){80, 80, len2, len, 0},
        (testcase_t){80, 80, len2, len2, 0},

        // slip y
        (testcase_t){80, 80, -len2, len2, 1},
        (testcase_t){80, 80, -len, len2, 1},
        (testcase_t){80, 80, -len2, len, 1},
        (testcase_t){80, 80, -len, len, 1},

        // slip x
        (testcase_t){80, 80, len2, -len2, 1},
        (testcase_t){80, 80, len, -len2, 1},
        (testcase_t){80, 80, len2, -len, 1},
        (testcase_t){80, 80, len, -len, 1},

        // upper-right corner
        // convex
        (testcase_t){130, 70, len, -len, 0},
        (testcase_t){130, 70, len, -len2, 0},
        (testcase_t){130, 70, len2, -len, 0},
        (testcase_t){130, 70, len2, -len2, 0},

        // concave
        (testcase_t){130, 70, -len, len, 0},
        (testcase_t){130, 70, -len, len2, 0},
        (testcase_t){130, 70, -len2, len, 0},
        (testcase_t){130, 70, -len2, len2, 0},

        // slip y
        (testcase_t){130, 70, len2, len2, 1},
        (testcase_t){130, 70, len, len2, 1},
        (testcase_t){130, 70, len2, len, 1},
        (testcase_t){130, 70, len, len, 1},

        // slip x
        (testcase_t){130, 70, -len2, -len2, 1},
        (testcase_t){130, 70, -len, -len2, 1},
        (testcase_t){130, 70, -len2, -len, 1},
        (testcase_t){130, 70, -len, -len, 1},

        // lower-left corner
        // convex
        (testcase_t){50, 150, -len, len, 0},
        (testcase_t){50, 150, -len, len2, 0},
        (testcase_t){50, 150, -len2, len, 0},
        (testcase_t){50, 150, -len2, len2, 0},

        // concave
        (testcase_t){50, 150, len, -len, 0},
        (testcase_t){50, 150, len, -len2, 0},
        (testcase_t){50, 150, len2, -len, 0},
        (testcase_t){50, 150, len2, -len2, 0},

        // slip y
        (testcase_t){50, 150, -len2, -len2, 1},
        (testcase_t){50, 150, -len, -len2, 1},
        (testcase_t){50, 150, -len2, -len, 1},
        (testcase_t){50, 150, -len, -len, 1},

        // slip x
        (testcase_t){50, 150, len2, len2, 1},
        (testcase_t){50, 150, len, len2, 1},
        (testcase_t){50, 150, len2, len, 1},
        (testcase_t){50, 150, len, len, 1},

        // lower-right corner
        // convex
        (testcase_t){140, 140, len, len, 0},
        (testcase_t){140, 140, len, len2, 0},
        (testcase_t){140, 140, len2, len, 0},
        (testcase_t){140, 140, len2, len2, 0},

        // concave
        (testcase_t){140, 140, -len, -len, 0},
        (testcase_t){140, 140, -len, -len2, 0},
        (testcase_t){140, 140, -len2, -len, 0},
        (testcase_t){140, 140, -len2, -len2, 0},

        // slip y
        (testcase_t){140, 140, len2, -len2, 1},
        (testcase_t){140, 140, len, -len2, 1},
        (testcase_t){140, 140, len2, -len, 1},
        (testcase_t){140, 140, len, -len, 1},

        // slip x
        (testcase_t){140, 140, -len2, len2, 1},
        (testcase_t){140, 140, -len, len2, 1},
        (testcase_t){140, 140, -len2, len, 1},
        (testcase_t){140, 140, -len, len, 1},
    };

    for (int t=0; t<n_cases; t++) {
        testcase_t tcase = data[t];

        // creating corner
        seg_a = SEG_init(tcase.x, tcase.y, tcase.x + tcase.wid, tcase.y);
        seg_b = SEG_init(tcase.x, tcase.y, tcase.x, tcase.y + tcase.hei);

        SEG_merge(&corner, seg_a);
        SEG_merge(&corner, seg_b);

        aux = LIG_generate_aux_hit_points(x1, y1, tcase.x, tcase.y, corner);
        ck_assert_int_eq(PT_len(aux), tcase.exp_seg);

        // CLEANUP
        corner = NULL;
    }
}
END_TEST

START_TEST (LIG_ray_intersect_check) 
{
    int n_cases = 38;

    typedef struct testcase {
          int o_x1;
          int o_y1;
          int o_x2;
          int o_y2;
          int r_x1;
          int r_y1;
          int r_x2;
          int r_y2;
          bool ex;
    } testcase_t;
    
    // GIVEN
    testcase_t data[] = {
        // 0-10
        (testcase_t){20, 50, 60, 50, 40, 100, 40, 20, true},
        (testcase_t){20, 50, 60, 50, 40, 100, 50, 20, true},
        (testcase_t){20, 50, 60, 50, 40, 100, 30, 20, true},
        (testcase_t){20, 50, 60, 50, 40, 100, 40, 60, false},
        (testcase_t){20, 50, 60, 50, 40, 100, 50, 60, false},

        (testcase_t){20, 50, 60, 50, 40, 100, 30, 60, false},
        (testcase_t){20, 50, 60, 50, 40, 100, 20, 50, true},
        (testcase_t){20, 50, 60, 50, 40, 100, 60, 50, true},
        (testcase_t){20, 50, 60, 50, 40, 0, 40, 60, true},
        (testcase_t){20, 50, 60, 50, 40, 0, 50, 60, true},

        // 11-20
        (testcase_t){20, 50, 60, 50, 40, 0, 30, 60, true},
        (testcase_t){20, 50, 60, 50, 40, 0, 40, 30, false},
        (testcase_t){20, 50, 60, 50, 40, 0, 50, 30, false},
        (testcase_t){20, 50, 60, 50, 40, 0, 30, 30, false},
        (testcase_t){20, 50, 60, 50, 40, 0, 20, 50, true},

        (testcase_t){20, 50, 60, 50, 40, 0, 60, 50, true},
        (testcase_t){20, 50, 20, 30, 10, 40, 60, 45, true},
        (testcase_t){20, 50, 20, 30, 10, 40, 60, 35, true},
        (testcase_t){20, 50, 20, 30, 10, 40, 60, 20, true},
        (testcase_t){20, 50, 20, 30, 90, 40, 0, 40, true},
        
        // 21-30
        (testcase_t){20, 50, 20, 30, 90, 40, 0, 45, true},
        (testcase_t){20, 50, 20, 30, 90, 40, 0, 35, true},
        (testcase_t){20, 50, 20, 30, 90, 40, 20, 50, true},
        (testcase_t){20, 50, 20, 30, 90, 40, 20, 30, true},
        (testcase_t){20, 50, 20, 30, 10, 40, 20, 50, true},

        (testcase_t){20, 50, 20, 30, 10, 40, 20, 30, true},
        (testcase_t){20, 50, 20, 30, 20, 40, 25, 40, true},
        (testcase_t){20, 50, 20, 30, 20, 40, 25, 50, true},
        (testcase_t){20, 50, 20, 30, 20, 40, 25, 30, true},
        (testcase_t){20, 50, 20, 30, 80, 40, 40, 40, false},
        
        // 31-38
        (testcase_t){20, 50, 20, 30, 80, 40, 40, 30, false},
        (testcase_t){20, 50, 20, 30, 80, 40, 40, 50, false},
        (testcase_t){20, 50, 20, 30, 20, 35, 20, 40, true},
        (testcase_t){20, 50, 20, 30, 20, 30, 20, 40, true},
        (testcase_t){50, 20, 50, 50, 50, 30, 50, 40, true},

        (testcase_t){50, 20, 50, 50, 50, 30, 50, 40, true},
        (testcase_t){50, 20, 50, 50, 50, 80, 50, 90, false},
        (testcase_t){20, 50, 40, 50, 60, 50, 90, 50, false},
    };

    // WHEN
    for (int i=36; i<n_cases; i++) {
        bool res;
        res = LIG_ray_intersect(
                data[i].o_x1,
                data[i].o_y1,
                data[i].o_x2,
                data[i].o_y2,
                data[i].r_x1,
                data[i].r_y1,
                data[i].r_x2,
                data[i].r_y2
        );
        ck_assert_int_eq(res, data[i].ex);
    }
}
END_TEST

START_TEST (LIG_find_closest_hit_segment_hor_check) 
{
    segment_t* inter = NULL;

    // there is not intersectional segments - no segment is found
    segment_t* res = LIG_find_closest_hit_segment_hor(inter, 0, 100, 100, 0);
    ck_assert_ptr_null(res); 

    // adding one simple segment - which should be hit!
    SEG_push(&inter, 10, 50, 90, 50);
    res = LIG_find_closest_hit_segment_hor(inter, 100, 0, 0, 100);

    ck_assert_int_eq(res->x1, 10);
    ck_assert_int_eq(res->y1, 50);
    ck_assert_int_eq(res->x2, 90);
    ck_assert_int_eq(res->y2, 50);

    // adding another segment which is further than the last one
    SEG_push(&inter, 10, 80, 90, 80);
    res = NULL; // sanity cleanup
    res = LIG_find_closest_hit_segment_hor(inter, 100, 0, 0, 100);

    ck_assert_int_eq(res->x1, 10);
    ck_assert_int_eq(res->y1, 50);
    ck_assert_int_eq(res->x2, 90);
    ck_assert_int_eq(res->y2, 50);

    // adding another segment which is closer than the last one
    SEG_push(&inter, 10, 20, 90, 20);
    res = NULL; // sanity cleanup
    res = LIG_find_closest_hit_segment_hor(inter, 100, 0, 0, 100);

    ck_assert_int_eq(res->x1, 10);
    ck_assert_int_eq(res->y1, 20);
    ck_assert_int_eq(res->x2, 90);
    ck_assert_int_eq(res->y2, 20);

    // checking collinear scenario
    inter = NULL;
    SEG_push(&inter, 10, 50, 90, 50);
    res = LIG_find_closest_hit_segment_hor(inter, 0, 50, 20, 50);
    ck_assert_int_eq(res->x1, 10);
    ck_assert_int_eq(res->y1, 50);
    ck_assert_int_eq(res->x2, 90);
    ck_assert_int_eq(res->y2, 50);

    // adding another collinear obstacle (closer)
    inter = NULL;
    SEG_push(&inter, 25, 50, 20, 50);
    res = NULL;
    res = LIG_find_closest_hit_segment_hor(inter, 0, 50, 20, 50);

    ck_assert_int_eq(res->x1, 25);
    ck_assert_int_eq(res->y1, 50);
    ck_assert_int_eq(res->x2, 20);
    ck_assert_int_eq(res->y2, 50);

    // putting obstacle inside rays begginig (it should clog the distance at zero)
    SEG_free(inter);
    inter = NULL;
    SEG_push(&inter, 0, 50, 10, 50);
    res = NULL;
    res = LIG_find_closest_hit_segment_hor(inter, 0, 50, 20, 50);

    ck_assert_int_eq(res->x1, 0);
    ck_assert_int_eq(res->y1, 50);
    ck_assert_int_eq(res->x2, 10);
    ck_assert_int_eq(res->y2, 50);
}
END_TEST

START_TEST (LIG_find_closest_hit_segment_ver_check) 
{
    segment_t* inter = NULL;

    // there is not intersectional segments - no segment is found
    segment_t* res = LIG_find_closest_hit_segment_ver(inter, 0, 100, 100, 0);

    // adding one simple segment - which should be hit!
    SEG_push(&inter, 50, 20, 50, 80);
    res = LIG_find_closest_hit_segment_ver(inter, 100, 0, 0, 100);

    ck_assert_int_eq(res->x1, 50);
    ck_assert_int_eq(res->y1, 20);
    ck_assert_int_eq(res->x2, 50);
    ck_assert_int_eq(res->y2, 80);

    // adding another segment which is further than the last one
    SEG_push(&inter, 30, 10, 30, 90);
    res = NULL; // sanity cleanup
    res = LIG_find_closest_hit_segment_ver(inter, 100, 0, 0, 100);

    ck_assert_int_eq(res->x1, 50);
    ck_assert_int_eq(res->y1, 20);
    ck_assert_int_eq(res->x2, 50);
    ck_assert_int_eq(res->y2, 80);

    // adding another segment which is closer than the last one
    SEG_push(&inter, 80, 10, 80, 90);
    res = NULL; // sanity cleanup
    res = LIG_find_closest_hit_segment_ver(inter, 100, 0, 0, 100);

    ck_assert_int_eq(res->x1, 80);
    ck_assert_int_eq(res->y1, 10);
    ck_assert_int_eq(res->x2, 80);
    ck_assert_int_eq(res->y2, 90);

    // checking collinear scenario
    SEG_free(inter);
    inter = NULL;
    SEG_push(&inter, 50, 10, 50, 90);

    res = LIG_find_closest_hit_segment_ver(inter, 0, 50, 20, 50);

    ck_assert_int_eq(res->x1, 50);
    ck_assert_int_eq(res->y1, 10);
    ck_assert_int_eq(res->x2, 50);
    ck_assert_int_eq(res->y2, 90);

    // adding another collinear obstacle (closer)
    SEG_free(inter);
    inter = NULL;
    SEG_push(&inter, 50, 5, 50, 30);
    res = NULL;
    res = LIG_find_closest_hit_segment_ver(inter, 0, 50, 20, 50);

    ck_assert_int_eq(res->x1, 50);
    ck_assert_int_eq(res->y1, 5); 
    ck_assert_int_eq(res->x2, 50);
    ck_assert_int_eq(res->y2, 30);

    // putting obstacle inside rays begginig (it should clog the distance at zero)
    SEG_free(inter);
    inter = NULL;
    SEG_push(&inter, 50, 0, 50, 15);
    res = NULL;
    res = LIG_find_closest_hit_segment_ver(inter, 10, 50, 20, 50);

    ck_assert_int_eq(res->x1, 50);
    ck_assert_int_eq(res->y1, 0); 
    ck_assert_int_eq(res->x2, 50);
    ck_assert_int_eq(res->y2, 15);
}
END_TEST

START_TEST (LIG_find_closest_hit_segment_check_single_segment) 
{
    // GIVEN
    typedef struct testcase {
          int r_x1;      
          int r_y1;      
          int r_x2;      
          int r_y2;      
          int o_x1;      
          int o_y1;      
          int o_x2;      
          int o_y2;      
          int exp_x;
          int exp_y;
    } testcase_t;
    
    int n_cases = 4;

    testcase_t data[] = {
        (testcase_t){20, 20, 40, 40, 30, 50, 30, 10, 30, 30},
        (testcase_t){20, 20, 40, 40, 0, 30, 100, 30, 30, 30},
        (testcase_t){40, 0, 10, 40, 30, 0, 30, 100, 30, 13}, 
        (testcase_t){40, 0, 10, 40, 0, 30, 100, 30, 17, 30},
    };

    // WHEN && THEN
    for (int i=0; i<n_cases; i++) {
        segment_t* inter = NULL;
        vertex_t* v = NULL;
        inter = SEG_init(data[i].o_x1, data[i].o_y1, data[i].o_x2, data[i].o_y2);

        v = LIG_find_closest_hit(inter, data[i].r_x1, data[i].r_y1, data[i].r_x2, data[i].r_y2);

        ck_assert_int_eq(v->x, data[i].exp_x);
        ck_assert_int_eq(v->y, data[i].exp_y);
    }
}
END_TEST

START_TEST (LIG_find_closest_hit_segment_check_multiple_segment) 
{
    // GIVEN
    typedef struct testcase {
          int r_x1;      
          int r_y1;      
          int r_x2;      
          int r_y2;      
          int h_x1;      
          int h_y1;      
          int h_x2;      
          int h_y2;      
          int v_x1;      
          int v_y1;      
          int v_x2;      
          int v_y2;      
          int exp_x;
          int exp_y;
    } testcase_t;
    
    int n_cases = 4;

    testcase_t data[] = {
        (testcase_t){20, 20, 50, 50, 0, 40, 100, 40, 20, 0, 20, 100, 20, 20},
        (testcase_t){20, 20, 50, 50, 0, 40, 100, 40, 25, 0, 25, 100, 25, 25},
        (testcase_t){20, 20, 50, 50, 0, 35, 100, 35, 40, 0, 40, 100, 35, 35},
        (testcase_t){20, 20, 50, 50, 0, 20, 100, 20, 40, 0, 40, 100, 20, 20},
    };

    // WHEN && THEN
    for (int i=0; i<n_cases; i++) {
        segment_t* hor = NULL;
        segment_t* ver = NULL;
        segment_t* inter = NULL;
        vertex_t* v = NULL;

        hor = SEG_init(data[i].v_x1, data[i].v_y1, data[i].v_x2, data[i].v_y2);
        ver = SEG_init(data[i].h_x1, data[i].h_y1, data[i].h_x2, data[i].h_y2);
        
        SEG_merge(&inter, hor);
        SEG_merge(&inter, ver);
        
        v = LIG_find_closest_hit(inter, data[i].r_x1, data[i].r_y1, data[i].r_x2, data[i].r_y2);
        ck_assert_int_eq(v->x, data[i].exp_x);
        ck_assert_int_eq(v->y, data[i].exp_y);
    }
}
END_TEST

START_TEST (LIG_find_closest_hit_segment_check_real_life_example) 
{
    // GIVEN
    typedef struct testcase {
          int r_x1;      
          int r_y1;      
          int r_x2;      
          int r_y2;      
          int h_x1;      
          int h_y1;      
          int h_x2;      
          int h_y2;      
          int v_x1;      
          int v_y1;      
          int v_x2;      
          int v_y2;      
          int exp_x;
          int exp_y;
    } testcase_t;

    testcase_t data[] = {
        (testcase_t){20, 20, 50, 50, 0, 40, 100, 40, 25, 0, 25, 100, 25, 25},
        (testcase_t){20, 20, 50, 50, 0, 35, 100, 35, 40, 0, 40, 100, 35, 35},
        (testcase_t){20, 20, 50, 50, 0, 20, 100, 20, 40, 0, 40, 100, 20, 20},
        (testcase_t){20, 20, 50, 50, 0, 40, 100, 40, 20, 0, 20, 100, 20, 20},
    };
    int n_cases = 4;

    // WHEN && THEN
    for (int i=0; i<n_cases; i++) {
        segment_t* hor = NULL;
        segment_t* ver = NULL;
        segment_t* inter = NULL;
        vertex_t* v = NULL;

        hor = SEG_init(data[i].v_x1, data[i].v_y1, data[i].v_x2, data[i].v_y2);
        ver = SEG_init(data[i].h_x1, data[i].h_y1, data[i].h_x2, data[i].h_y2);
        
        SEG_merge(&inter, hor);
        SEG_merge(&inter, ver);

        v = LIG_find_closest_hit(inter, data[i].r_x1, data[i].r_y1, data[i].r_x2, data[i].r_y2);
        
        ck_assert_int_eq(v->x, data[i].exp_x);
        ck_assert_int_eq(v->y, data[i].exp_y);
    }
}
END_TEST

START_TEST (LIG_find_closest_hit_segment_check_real_life_example2) 
{
    // GIVEN
    segment_t* inter = NULL;
    vertex_t*  v     = NULL;
    
    int r_x1, r_y1, r_x2, r_y2;
    int exp_x, exp_y;

    // CASE 1 
    SEG_push(&inter, 20, 0, 20, 30);
    SEG_push(&inter, 30, 0, 50, 0);
    SEG_push(&inter, 70, 0, 70, 20);
    SEG_push(&inter, 80, 0, 90, 0);

    r_x1 = 100; r_y1 = 0; r_x2 = 0; r_y2 = 0;
    exp_x = 90; exp_y = 0;
    v = LIG_find_closest_hit(inter, r_x1, r_y1, r_x2, r_y2);
    
    ck_assert_int_eq(v->x, exp_x); ck_assert_int_eq(v->y, exp_y);
    inter = NULL; v = NULL;

    // CASE 2 
    SEG_push(&inter, 20, 0, 20, 30);
    SEG_push(&inter, 30, 0, 50, 0);
    SEG_push(&inter, 70, 0, 70, 20);
    SEG_push(&inter, 80, 0, 90, 0);

    r_x1 = 0; r_y1 = 0; r_x2 = 100; r_y2 = 0;
    exp_x = 20; exp_y = 0;
    v = LIG_find_closest_hit(inter, r_x1, r_y1, r_x2, r_y2);
    
    ck_assert_int_eq(v->x, exp_x); ck_assert_int_eq(v->y, exp_y);
    inter = NULL; v = NULL;

    // CASE 3
    SEG_push(&inter, 10, 90, 10, 80); // ver
    SEG_push(&inter, 10, 60, 40, 60); // hor
    SEG_push(&inter, 10, 50, 10, 30); // ver
    SEG_push(&inter, 10, 10, 50, 10); // hor

    r_x1 = 10; r_y1 = 100; r_x2 = 10; r_y2 = 0;
    exp_x = 10; exp_y = 90;
    v = LIG_find_closest_hit(inter, r_x1, r_y1, r_x2, r_y2);
    
    ck_assert_int_eq(v->x, exp_x); ck_assert_int_eq(v->y, exp_y);
    inter = NULL; v = NULL;

    // CASE 4
    SEG_push(&inter, 10, 90, 10, 80); // ver
    SEG_push(&inter, 10, 60, 40, 60); // hor
    SEG_push(&inter, 10, 50, 10, 30); // ver
    SEG_push(&inter, 10, 20, 50, 20); // hor

    r_x1 = 10; r_y1 = 0; r_x2 = 10; r_y2 = 100;
    exp_x = 10; exp_y = 20;
    v = LIG_find_closest_hit(inter, r_x1, r_y1, r_x2, r_y2);
    
    ck_assert_int_eq(v->x, exp_x); ck_assert_int_eq(v->y, exp_y);
    inter = NULL; v = NULL;

    // CASE 5
    SEG_push(&inter, 60, 60, 60, 100);
    SEG_push(&inter, 70, 60, 90, 60);
    SEG_push(&inter, 80, 0, 80, 50);

    r_x1 = 100; r_y1 = 0; r_x2 = 0; r_y2 = 100;
    exp_x = 80; exp_y = 20;
    v = LIG_find_closest_hit(inter, r_x1, r_y1, r_x2, r_y2);
    
    ck_assert_int_eq(v->x, exp_x); ck_assert_int_eq(v->y, exp_y);
    inter = NULL; v = NULL;
}
END_TEST

START_TEST (LIG_generate_hit_points_check) 
{
    int x = 90;
    int y = 90;
    int x2 = 0; // just to set angle right
    int y2 = 0; // just to set angle right

    typedef struct testcase {
          float width;
          int expected_n_pts;
    } testcase_t;
    
    int n_cases = 5;

    testcase_t data[] = {
        (testcase_t){DEG45, 5},
        (testcase_t){PI/3,  7},
        (testcase_t){DEG15, 3},
        (testcase_t){DEG1,  2},
        (testcase_t){0.0,   8},
    };

    float angle = GEO_angle_2pt(x, y, x2, y2);

    segment_t* obstacles = NULL;

    // border
    SEG_push(&obstacles, 0,   0,   100, 0  );   // upper 
    SEG_push(&obstacles, 0,   100, 100, 100);   // donwer
    SEG_push(&obstacles, 0,   0,   0,   100);   // left
    SEG_push(&obstacles, 100, 0,   100, 100);   // right
    
    // obstacles
    SEG_push(&obstacles, 10, 10, 20, 10);
    SEG_push(&obstacles, 20, 10, 20, 50);
    SEG_push(&obstacles, 20, 50, 10, 50);
    SEG_push(&obstacles, 10, 50, 10, 10);

    for (int i=0; i<n_cases; i++) {
        point_t* hits     = NULL;

        hits = LIG_generate_hit_points(x, y, data[i].width, angle, obstacles);
        ck_assert_int_eq(PT_len(hits), data[i].expected_n_pts);
    }
}
END_TEST

Suite *light_suite(void) {
    Suite *s;
    TCase *tc_core;
    s = suite_create("light");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, LIG_ray_intersects_multiple_check_border_case);
    tcase_add_test(tc_core, LIG_ray_intersects_multiple_check_hit_into_corner);
    tcase_add_test(tc_core, LIG_ray_intersects_multiple_check_hit_into_reversed_corner);
    tcase_add_test(tc_core, LIG_ray_intersects_multiple_check_multiple_obstacles);
    tcase_add_test(tc_core, LIG_ray_intersect_check);
    tcase_add_test(tc_core, LIG_ray_intersects_multiple_check_hit_collinear_obstacles);

    tcase_add_test(tc_core, LIG_generate_slipover_hit_point_check);
    tcase_add_test(tc_core, LIG_generate_aux_hit_points_check_ray_inside_corner_aux_pt_generated);
    tcase_add_test(tc_core, LIG_generate_aux_hit_points_edge_case);
    tcase_add_test(tc_core, LIG_generate_hit_points_check);
    tcase_add_test(tc_core, LIG_calculate_dull_ray_end_check);

    tcase_add_test(tc_core, LIG_find_closest_hit_segment_check_single_segment);
    tcase_add_test(tc_core, LIG_find_closest_hit_segment_ver_check);
    tcase_add_test(tc_core, LIG_find_closest_hit_segment_hor_check);
    tcase_add_test(tc_core, LIG_find_closest_hit_segment_check_multiple_segment);
    tcase_add_test(tc_core, LIG_find_closest_hit_segment_check_real_life_example);
    tcase_add_test(tc_core, LIG_find_closest_hit_segment_check_real_life_example2);

    tcase_add_test(tc_core, LIG_get_base_light_polygon_check);
    tcase_add_test(tc_core, LIG_get_light_polygon_check);

    suite_add_tcase(s, tc_core);

    return s;
}
