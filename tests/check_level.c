#include <check.h>
#include <stdlib.h>
#include "../src/primitives.h"
#include "../src/level.h"
#include "../src/segment.h"


START_TEST (LVL_analyze_check)
{
    // GIVEN 
    // lets put hero on the middle
    const int size_x = 10; 
    const int size_y = 10;

    int tile_width = 32;
    int tile_height = 32;
    int hero_x = (int)(size_x * tile_width / 2);
    int hero_y = (int)(size_x * tile_height / 2);
    int len = 0;  
    int expected_obstacles_num;

    level_t* test_level = NULL;
    test_level = LVL_new();

    test_level->size_y = 10;
    test_level->size_x = 10;

    // CASE A (single long platform)
    // 4 screen borders + 4 obstacle border
    expected_obstacles_num = 4 + 4;
    int obstacles_a[100] =  {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    test_level->obstacles = obstacles_a;
    LVL_analyze(test_level, hero_x, hero_y);
    len = SEG_len(test_level->obstacle_segments);

    ck_assert_int_eq(len, expected_obstacles_num);
    
    // CASE B (single tall platform)
    // 4 screen borders + 4 obstacle border
    expected_obstacles_num = 4 + 4;
    int obstacles_b[100] =  {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    test_level->obstacles = obstacles_b;
    LVL_analyze(test_level, hero_x, hero_y);
    len = SEG_len(test_level->obstacle_segments);

    ck_assert_int_eq(len, expected_obstacles_num);

    // CASE C (two separate platforms)
    // 4 screen borders + 4 first obstacle borders + 4 second obstacle boder
    expected_obstacles_num = 4 + 4 + 4;
    int obstacles_c[100] =  {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    test_level->obstacles = obstacles_c;
    LVL_analyze(test_level, hero_x, hero_y);
    len = SEG_len(test_level->obstacle_segments);

    ck_assert_int_eq(len, expected_obstacles_num);

    // CASE D (many platforms with some strange constructions)
    // 4 level borders + 14 platform borders
    expected_obstacles_num = 18;
    int obstacles_d[100] =  {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        0, 1, 0, 0, 0, 1, 0, 0, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    test_level->obstacles = obstacles_d;
    LVL_analyze(test_level, hero_x, hero_y);
    len = SEG_len(test_level->obstacle_segments);

    ck_assert_int_eq(len, expected_obstacles_num);
}
END_TEST


Suite *level_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("level");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, LVL_analyze_check);

    suite_add_tcase(s, tc_core);

    return s;
}
