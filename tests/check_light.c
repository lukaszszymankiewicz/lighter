#include <check.h>
#include <stdlib.h>
#include <math.h>
#include "../src/light.h"
#include "../src/obstacle.h"
#include "../src/vertex.h"
#include "../src/point.h"
#include "../src/macros.h"


START_TEST (LIG_get_visible_obstacles_check) 
{
    // GIVEN
    obstacle_t* obstacles = NULL;
    obstacle_t* visible_obstacles = NULL;
    int expected_len = 5;

    int x = 80; 
    int y = 90; 
    float width = PI / 4; // 45%
    float angle = LEFT_RAD;

    // border
    OBS_push(&obstacles, 0,   0,   100, 0  );   // upper 
    OBS_push(&obstacles, 0,   100, 100, 100);   // donwer
    OBS_push(&obstacles, 0,   0,   0,   100);   // left
    OBS_push(&obstacles, 100, 0,   100, 100);   // right
    
    // obstacles
    OBS_push(&obstacles, 10, 10, 20, 10);
    OBS_push(&obstacles, 20, 10, 20, 50);
    OBS_push(&obstacles, 20, 50, 10, 50);
    OBS_push(&obstacles, 10, 50, 10, 10);

    // THEN
    visible_obstacles = LIG_get_visible_obstacles(obstacles, y, x, angle, width);
    int calculated_length = OBS_len(visible_obstacles);
    
    ck_assert_int_eq(calculated_length, expected_len);
}
END_TEST

START_TEST (LIG_calc_hit_points_real_life_example_check) 
{
    // GIVEN
    obstacle_t *obstacles = NULL;
    point_t    *points    = NULL;

    // four vertex of obstacle vertices, two border points on edge of slight plus one vertex of
    // screen
    int expected_len = 4 + 2 + 1;

    int x = SCREEN_WIDTH / 2;
    int y = 144;
    float width = 0.448786;
    float angle = -0.625243;

    OBS_push(&obstacles, 0,   0,  320, 0);
    OBS_push(&obstacles, 320, 0,  320, 200);
    OBS_push(&obstacles, 274, 4,  274, 36);
    OBS_push(&obstacles, 242, 4,  274, 4);
    OBS_push(&obstacles, 242, 36, 274, 36);

    // THEN
    points = LIG_calc_hit_points(x, y, angle,  width, obstacles);
    int calculated_length = PT_len(points);
    
    ck_assert_int_eq(calculated_length, expected_len);
}
END_TEST

START_TEST (LIG_calc_hit_points_check) 
{
    // GIVEN
    obstacle_t *obstacles = NULL;
    point_t    *points    = NULL;

    int expected_len = 5;

    int x = 80; 
    int y = 90; 
    float width = PI / 4; // 45%
    float angle = LEFT_RAD;

    // border
    OBS_push(&obstacles, 0,   0,   100, 0  );   // upper 
    OBS_push(&obstacles, 0,   100, 100, 100);   // donwer
    OBS_push(&obstacles, 0,   0,   0,   100);   // left
    OBS_push(&obstacles, 100, 0,   100, 100);   // right
    
    // obstacles
    OBS_push(&obstacles, 10, 10, 20, 10);
    OBS_push(&obstacles, 20, 10, 20, 50);
    OBS_push(&obstacles, 20, 50, 10, 50);
    OBS_push(&obstacles, 10, 50, 10, 10);

    // THEN
    points = LIG_calc_hit_points(x, y, angle,  width, obstacles);
    int calculated_length = PT_len(points);
    
    ck_assert_int_eq(calculated_length, expected_len);

}
END_TEST

START_TEST (LIG_calc_light_polygon_check) 
{
    // GIVEN
    obstacle_t* obstacles = NULL;
    vertex_t* light_vertex = NULL;

    int expected_len = (4 + 1)*2 +1;

    int x = 80; 
    int y = 90; 
    float width = PI / 4; // 45%
    float angle = LEFT_RAD;

    // border
    OBS_push(&obstacles, 0,   0,   100, 0  );   // upper 
    OBS_push(&obstacles, 0,   100, 100, 100);   // donwer
    OBS_push(&obstacles, 0,   0,   0,   100);   // left
    OBS_push(&obstacles, 100, 0,   100, 100);   // right
    
    // obstacles
    OBS_push(&obstacles, 10, 10, 20, 10);
    OBS_push(&obstacles, 20, 10, 20, 50);
    OBS_push(&obstacles, 20, 50, 10, 50);
    OBS_push(&obstacles, 10, 50, 10, 10);

    // THEN
    light_vertex = LIG_calc_light_polygon(x, y, angle,  width, obstacles);
    int calculated_length = VRTX_len(light_vertex );
    
    vertex_t* ptr = NULL; 
    ptr = light_vertex ;

    int found;  

    while(ptr) {
    // every light point must belong to some obstacle segment (no point inside or outside).
        found = 0;
        
        obstacle_t* o = NULL; 
        o = obstacles; 

        while(o) {
            if (ptr->y == y && ptr->x == x){
                    found = 1; //player poisiton
                    break;
            }

            if (o->x1 == o->x2) {
                if (ptr->x == o->x1 && ptr->y <= MAX(o->y1, o->y2) && ptr->y >= MIN(o->y1, o->y2)) {
                    found = 1;
                }
            }
            else if (o->y1 == o->y2) {
                if (ptr->y == o->y1 && ptr->x <= MAX(o->x1, o->x2) && ptr->x >= MIN(o->x1, o->x2)) {
                    found = 1;
                }
            }

            if (found) {
                break;
            }
            else {
                o = o->next;
            }
        }
        ck_assert_int_eq(found, 1);
        ptr=ptr->next;
    }
    ck_assert_int_eq(calculated_length, expected_len);
}
END_TEST

 
START_TEST (LIG_ray_hits_obstacle_vertical_pass) 
{
    // GIVEN
    int x1; int y1;
    int x2; int y2;
    point_t* result_pt = NULL;
    
    int n_tests = 4;

    int x1s[4] = {  0, 20, 100, 60};
    int y1s[4] = {100,  0, 70,  80};
    int x2s[4] = {60,  70, 30,   2};
    int y2s[4] = {70,  50, 40,  40};
    
    obstacle_t* obstacle = OBS_init(50, 20, 50, 80);

    for (int i=0; i<n_tests; i++) {
        x1 = x1s[i];
        y1 = y1s[i];
        x2 = x2s[i];
        y2 = y2s[i];

        result_pt = LIG_ray_hits_obstacle(x1, y1, x2, y2,  obstacle);

        ck_assert_int_eq(result_pt->x, obstacle->x1);
        ck_assert_int_eq(result_pt->x, obstacle->x2);
        ck_assert_int_ge(result_pt->y, obstacle->y1);
        ck_assert_int_le(result_pt->y, obstacle->y2);
    }
}
END_TEST
 
START_TEST (LIG_ray_hits_obstacle_vertical_fail) 
{
    // GIVEN
    int x1; int y1;
    int x2; int y2;
    point_t* result_pt = NULL;
    
    int n_tests = 4;

    int x1s[4] = {40, 40,  20, 40};
    int y1s[4] = {80, 100, 40, 60};
    int x2s[4] = {60, 60,  20, 20};
    int y2s[4] = {90, 90,  40, 70};
    
    obstacle_t* obstacle = OBS_init(50, 20, 50, 80);

    for (int i=0; i<1; i++) {
        x1 = x1s[i];
        y1 = y1s[i];
        x2 = x2s[i];
        y2 = y2s[i];

        result_pt = LIG_ray_hits_obstacle(x1, y1, x2, y2,  obstacle);

        ck_assert_int_eq(result_pt->x, x2s[i]);
        ck_assert_int_eq(result_pt->y, y2s[i]);

    }
}
END_TEST

START_TEST (LIG_ray_hits_obstacle_vertical_pass_edge_cases) 
{
    point_t* result_pt = NULL;
    int x1, y1, x2, y2;
    int n_tests = 2;

    int x1s[4] = {10,  10};
    int y1s[4] = {10,  10};
    int x2s[4] = {50,  50};
    int y2s[4] = {20,  80};
    
    obstacle_t* obstacle = OBS_init(50, 20, 50, 80);

    for (int i=0; i<n_tests; i++) {
        x1 = x1s[i];
        y1 = y1s[i];
        x2 = x2s[i];
        y2 = y2s[i];

        result_pt = LIG_ray_hits_obstacle(x1, y1, x2, y2,  obstacle);

        ck_assert_int_eq(result_pt->x, x2s[i]);
        ck_assert_int_eq(result_pt->y, y2s[i]);
    }
}
END_TEST


START_TEST (LIG_ray_hits_obstacle_horizontal_pass) 
{
    // GIVEN
    int x1; int y1;
    int x2; int y2;
    point_t* result_pt = NULL;
    
    int n_tests = 4;

    int x1s[4] = { 0, 70, 15, 60};
    int y1s[4] = {10,  0, 70, 90};
    int x2s[4] = {60, 30, 30, 70};
    int y2s[4] = {70, 70, 40,  2};
    
    obstacle_t* obstacle = OBS_init(20, 50, 80, 50);

    for (int i=0; i<n_tests; i++) {
        x1 = x1s[i];
        y1 = y1s[i];
        x2 = x2s[i];
        y2 = y2s[i];

        result_pt = LIG_ray_hits_obstacle(x1, y1, x2, y2,  obstacle);

        ck_assert_int_eq(result_pt->y, obstacle->y1);
        ck_assert_int_eq(result_pt->y, obstacle->y2);
        ck_assert_int_ge(result_pt->x, obstacle->x1);
        ck_assert_int_le(result_pt->x, obstacle->x2);

    }
}
END_TEST


START_TEST (LIG_ray_hits_obstacle_horizontal_fail) 
{

    // GIVEN
    int x1; int y1;
    int x2; int y2;
    point_t* result_pt = NULL;
    
    int n_tests = 4;

    int x1s[4] = {10,  10, 10, 10};
    int y1s[4] = {10,  10, 10, 10};
    int x2s[4] = {100, 10, 60,  0};
    int y2s[4] = {60,  70,  0,  0};
    
    obstacle_t* obstacle = OBS_init(20, 50, 80, 50);

    for (int i=0; i<n_tests; i++) {
        x1 = x1s[i];
        y1 = y1s[i];
        x2 = x2s[i];
        y2 = y2s[i];

        result_pt = LIG_ray_hits_obstacle(x1, y1, x2, y2,  obstacle);

        ck_assert_int_eq(result_pt->x, x2s[i]);
        ck_assert_int_eq(result_pt->y, y2s[i]);
    }
}
END_TEST

START_TEST (LIG_ray_hits_obstacle_horizontal_edge_cases) 
{
    // GIVEN
    int x1; int y1;
    int x2; int y2;
    point_t* result_pt = NULL;
    
    int n_tests = 2;

    int x1s[4] = {10,  10};
    int y1s[4] = {10,  10};
    int x2s[4] = {20,  80};
    int y2s[4] = {50,  50};
    
    obstacle_t* obstacle = OBS_init(20, 50, 80, 50);

    for (int i=0; i<n_tests; i++) {
        x1 = x1s[i];
        y1 = y1s[i];
        x2 = x2s[i];
        y2 = y2s[i];

        result_pt = LIG_ray_hits_obstacle(x1, y1, x2, y2, obstacle);

        ck_assert_int_eq(result_pt->x, x2s[i]);
        ck_assert_int_eq(result_pt->y, y2s[i]);
    }
}
END_TEST

START_TEST (LIG_closest_intersection_with_obstacle_check)  
{
    // GIVEN
    int x1 = 10;
    int y1 = 40;
    int x2 = 100;
    int y2 = 60;
    point_t* result_pt = NULL;
    
    int expected_hit_x  = 30;

    obstacle_t* obstacles = OBS_init(20, 50, 80, 50);

    OBS_push(&obstacles, expected_hit_x, 30, expected_hit_x, 70);
    OBS_push(&obstacles, 50, 20, 50, 80);
    OBS_push(&obstacles, 60, 10, 60, 90);
    OBS_push(&obstacles, 60, 10, 60, 90);
    OBS_push(&obstacles, 40, 60, 40, 100);

    result_pt = LIG_closest_intersection_with_obstacle(x1, y1, x2, y2, obstacles);

    ck_assert_int_eq(result_pt->x, expected_hit_x);
    ck_assert_int_le(result_pt->x, 70);
    ck_assert_int_ge(result_pt->x, 30);

}
END_TEST

START_TEST (LIG_any_intersection_with_obstacle_check)  
{
    // GIVEN
    int x1 = 10;
    int y1 = 10;
    int x2 = 60;
    int y2 = 100;
    bool result_pt = NULL;
    obstacle_t* obstacles = NULL;

    OBS_push(&obstacles, 60, 0, 60, 100);
    result_pt = LIG_any_intersection_with_obstacle(x1, y1, x2, y2, obstacles);
    ck_assert_int_eq(result_pt, 0);

    OBS_push(&obstacles, 20, 0, 20, 100);
    result_pt = LIG_any_intersection_with_obstacle(x1, y1, x2, y2, obstacles);
    ck_assert_int_eq(result_pt, 1);

    OBS_push(&obstacles, 40, 0, 40, 200);
    result_pt = LIG_any_intersection_with_obstacle(x1, y1, x2, y2, obstacles);
    ck_assert_int_eq(result_pt, 1);
}
END_TEST

Suite *light_suite(void) {
    Suite *s;
    TCase *tc_core;
    s = suite_create("light");


    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, LIG_get_visible_obstacles_check);
    tcase_add_test(tc_core, LIG_calc_light_polygon_check);
    tcase_add_test(tc_core, LIG_calc_hit_points_check);
    tcase_add_test(tc_core, LIG_calc_hit_points_real_life_example_check);

    // vertical lines
    tcase_add_test(tc_core, LIG_ray_hits_obstacle_vertical_pass);
    tcase_add_test(tc_core, LIG_ray_hits_obstacle_vertical_fail);
    tcase_add_test(tc_core, LIG_ray_hits_obstacle_vertical_pass_edge_cases);

    // horizontal lines
    tcase_add_test(tc_core, LIG_ray_hits_obstacle_horizontal_pass);
    tcase_add_test(tc_core, LIG_ray_hits_obstacle_horizontal_fail);
    tcase_add_test(tc_core, LIG_ray_hits_obstacle_horizontal_edge_cases);

    // hits
    tcase_add_test(tc_core, LIG_closest_intersection_with_obstacle_check);
    tcase_add_test(tc_core, LIG_any_intersection_with_obstacle_check);

    suite_add_tcase(s, tc_core);

    return s;
}
