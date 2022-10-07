#include <check.h>
#include <stdlib.h>
#include "../src/files.h"
#include "../src/sprites.h"


START_TEST (TXTR_init_animation_sheet_check)
{
    // GIVEN 
    char* filepath = HERO_ANIMATION_SHEET_FILE;

    // WHEN
    animation_sheet_t* sheet = TXTR_init_animation_sheet(filepath);

    // THEN
    ck_assert_ptr_nonnull(sheet);
}
END_TEST


Suite *sprites_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("sprites");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, TXTR_init_animation_sheet_check);

    suite_add_tcase(s, tc_core);

    return s;
}

