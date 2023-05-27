#include <check.h>

#include "../src/game.h"

START_TEST (GAME_run_check)
{
    // GIVEN
    int max_frames     = 1;
    int graphic_option = GRAPHIC_OFF;
    int level_id       = LEVEL_SAMPLE;

    // WHEN && THEN
    GAME_run(LEVEL_SAMPLE, GRAPHIC_OFF, max_frames);
}
END_TEST

Suite *game_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("game");

    /* Core test case */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, GAME_run_check);
    suite_add_tcase(s, tc_core);

    return s;
}
