#include <check.h>
#include "../src/files.h"
#include "../src/primitives.h"
#include "../src/import.h"
#include "../src/level.h"
#include "../src/gfx.h"
#include "../src/segment.h"
#include "../src/animation.h"


START_TEST (LVL_analyze_check)
{
    // GIVEN 
    // lets put hero on the middle
    const int size_x = 10; 
    const int size_y = 10;

    int len = 0;  
    int expected_obstacles_num;

    level_t* test_level = NULL;
    test_level = LVL_new();

    test_level->size_y = size_y;
    test_level->size_x = size_x;

    // CASE A (single long platform)
    // 4 screen borders
    expected_obstacles_num = 4;
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
    LVL_analyze(test_level);
    len = SEG_len(test_level->obstacle_segments);

    ck_assert_int_eq(len, expected_obstacles_num);
    
    // CASE B (single tall platform)
    // 4 screen borders
    expected_obstacles_num = 4;
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
    LVL_analyze(test_level);
    len = SEG_len(test_level->obstacle_segments);

    ck_assert_int_eq(len, expected_obstacles_num);

    // CASE C (two separate platforms)
    // 4 screen borders + 4 second obstacle boder
    expected_obstacles_num = 4 + 4;
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
    LVL_analyze(test_level);
    len = SEG_len(test_level->obstacle_segments);

    ck_assert_int_eq(len, expected_obstacles_num);

    // CASE D (many platforms with some strange constructions)
    // 14 platform borders
    expected_obstacles_num = 14;
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
    LVL_analyze(test_level);
    len = SEG_len(test_level->obstacle_segments);

    ck_assert_int_eq(len, expected_obstacles_num);
}
END_TEST
START_TEST (LVL_fill_structure_check)
{
    // GIVEN
    texture_t *tex   = NULL;
    level_t   *level = NULL;
    char *tex_filepath = "./tests/testfiles/testlevel/level.png";

    level = LVL_new();
    tex   = TXTR_read_from_file(tex_filepath);
    
    level->tileset = tex;
    LVL_set_size(level, 10, 10);

    // WHEN
    LVL_fill_structure(level, 5, 5, 0);
    tile_t *tile = NULL;
    tile         = LVL_tile_on_pos(level, 5, 5);

    // check if LVL_fill_structure is sucessful
    ck_assert_ptr_nonnull(tile);

    ck_assert_float_eq(tile->coord.x1, 0.5);
    ck_assert_float_eq(tile->coord.y1, (2.0/3.0 * -1.0 ));
    ck_assert_float_eq(tile->coord.x2, 0.6);
    ck_assert_float_eq(tile->coord.y2, -0.8);

    ck_assert_float_eq(tile->img.x1, 0.0);
    ck_assert_float_eq(tile->img.y1, 0.0);
    ck_assert_float_eq(tile->img.x2, 0.1);
    ck_assert_float_eq(tile->img.y2, (32.0 / 224.0));

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
    tcase_add_test(tc_core, LVL_fill_structure_check);

    suite_add_tcase(s, tc_core);

    return s;
}
