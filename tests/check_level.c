#include <check.h>
#include <stdlib.h>
#include "../src/primitives.h"
#include "../src/level.h"
#include "../src/gfx.h"
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

    test_level->size_y = size_y;
    test_level->size_x = size_x;

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


START_TEST (LVL_fill_tiles_check)
{
    // GIVEN
    texture_t* tex = NULL;
    level_t *level = NULL;
    char *tex_filepath = "./tests/testfiles/testlevel/level.png";

    level = LVL_new();
    tex = GFX_read_texture(tex_filepath);
    int cols = (int)tex->width/TILE_WIDTH;

    LVL_set_tileset(level, tex);

    // WHEN
    LVL_fill_tiles(level);

    // THEN
    // test texture is 320*224 image resulting in 10 * 7 = 70 tiles. Each of them should be read and
    // placed in tile_array.
    for (int i=0; i<70; i++) {
        ck_assert_ptr_nonnull(&(level->tile_array[i]));
        tile_t *tile = &(level->tile_array[i]);
        int x = (int)i%cols;
        int y = (int)i/cols;
        ck_assert_int_eq(tile->x, x*TILE_WIDTH);
        ck_assert_int_eq(tile->y, y*TILE_HEIGHT);
    }
}
END_TEST


START_TEST (LVL_fill_structure_check)
{
    // GIVEN
    texture_t* tex = NULL;
    level_t *level = NULL;
    char *tex_filepath = "./tests/testfiles/testlevel/level.png";

    level = LVL_new();
    tex = GFX_read_texture(tex_filepath);
    LVL_set_tileset(level, tex);
    LVL_set_size(level, 10, 10);
    LVL_set_tile_number(level, 2);
    LVL_add_tile(level, 0, 0, 0);
    LVL_add_tile(level, 1, 32, 0);

    // WHEN
    LVL_fill_structure(level, 5, 5, 0);
    tile_t *tile = LVL_tile_on_pos(level, 5, 5);

    LVL_fill_structure(level, 6, 6, 1);
    tile_t *tile_b = LVL_tile_on_pos(level, 6, 6);

    LVL_fill_structure(level, 7, 7, 1);
    tile_t *tile_c = LVL_tile_on_pos(level, 7, 7);

    // THEN
    tile_t *ex_tile = &level->tile_array[0];
    tile_t *ex_tile_b = &level->tile_array[1];
    tile_t *ex_tile_c = &level->tile_array[1];

    ck_assert_ptr_nonnull(tile);
    ck_assert_ptr_nonnull(tile_b);
    ck_assert_ptr_nonnull(tile_c);

    ck_assert_ptr_eq(ex_tile, tile);
    ck_assert_ptr_eq(ex_tile_b, tile_b);
    ck_assert_ptr_eq(ex_tile_c, tile_c);
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
    tcase_add_test(tc_core, LVL_fill_tiles_check);
    tcase_add_test(tc_core, LVL_fill_structure_check);

    suite_add_tcase(s, tc_core);

    return s;
}
