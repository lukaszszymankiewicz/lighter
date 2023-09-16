#include <check.h>

#include "../src/animation.h"
#include "../src/level.h"
#include "../src/segment.h"

#include "../src/data/library.h"

#define MAX_OBSTACLE_TILES 25

int obstacle    = 2; // tile with such index is considered as obstacle
int no_obstacle = 0; // tile with such index is considered as no obstacle

void UNTIL_clear_tiles(
    int id
) {
    int size_x = levels_library[LEVEL_TEST]->size_x;
    int size_y = levels_library[LEVEL_TEST]->size_y;

    for (int x=0; x<size_x; x++) {
        for (int y=0; y<size_y; y++) {
            levels_library[LEVEL_TEST]->tiles[y * size_x + x] = no_obstacle;
        }
    }
}

START_TEST (LVL_analyze_check)
{
    typedef struct testcase {
          int expected_obstacles_num;
          int obstacle_tiles_n;
          int obstacle_tiles_idx[MAX_OBSTACLE_TILES];
    } testcase_t;

    int n_cases = 4;
    testcase_t testcases[] = {
         {4, 4, {80, 81, 82, 83, -1} }, // single long platform
         {4, 4, {21, 31, 41, 51, -1} }, // single long platform
         {4, 4, {21, 31, 41, 51, 81, 82, 83, 84, -1} }, // two platforms
         {14, 20, { 35, 45, 55, 65, 21, 31, 41, 51, 61, 71, 72, 73, 74, 75, 76, 77, 78, 88, 68, 58, -1 } }
    };

    for (int i=0; i<n_cases; i++) {
        // prepare library
        LIB_create_all();
        UNTIL_clear_tiles(LEVEL_TEST);

        // prepare level
        for (int tile=0; tile<testcases[i].obstacle_tiles_n; tile++) {
            int id = testcases[i].obstacle_tiles_idx[tile];
            if (id  == -1) { break; }
            levels_library[LEVEL_TEST]->tiles[id] = obstacle;
        }

        // run analyze function
        level_t* test_level = NULL;
        test_level          = LVL_new(LEVEL_TEST);
        LVL_analyze(test_level);
        
        // assertions
        int len = SEG_len(test_level->obstacle_segments);
        ck_assert_int_eq(len, testcases[i].expected_obstacles_num);
    }
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
