#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <check.h>
#include "../src/level.h"
#include "../src/import.h"
#include "../src/files.h"

START_TEST (IMP_read_level_check_negative)
{
    // GIVEN
    level_t *level = NULL;
    char *filename =    "./nonexisintg";

    // WHEN
    level = IMP_read_level(filename);

    // THEN
    ck_assert_ptr_null(level);
}
END_TEST
 
START_TEST (IMP_read_level_check_positive)
{
    // GIVEN
    level_t *level = NULL;
    char *filename =    "./tests/testfiles/testlevel";

    int expected_x_size = 40;
    int expected_y_size = 40;
    int expected_x_hero = 6;
    int expected_y_hero = 3;

    // WHEN
    level = IMP_read_level(filename);

    // THEN
    ck_assert_int_eq(level->size_x, expected_x_size);
    ck_assert_int_eq(level->size_y, expected_y_size);

    ck_assert_int_eq(level->hero_x, expected_x_hero);
    ck_assert_int_eq(level->hero_y, expected_y_hero);
}
END_TEST
 
START_TEST (IMP_read_animation_check)
{
    // GIVEN
    animation_sheet_t *sheet = NULL;
    char *data               = HERO_ANIMATION_SHEET_DATA;
    char *img                = HERO_ANIMATION_SHEET_FILE;

    // WHEN
    sheet = IMP_read_animation(img, data);

    // THEN
    ck_assert_ptr_nonnull(sheet);
    ck_assert_ptr_nonnull(sheet->texture);

    // sheet
    ck_assert_int_eq(sheet->n_animations, 2);

    // single animation
    ck_assert_int_eq(sheet->animations[0].len, 2);

    // single frame
    // first animation 
    // first frame
    ck_assert_int_eq(sheet->animations[0].frames[0].delay, 20);

    ck_assert_int_eq(sheet->animations[0].frames[0].rect.x, 0);
    ck_assert_int_eq(sheet->animations[0].frames[0].rect.y, 0);
    ck_assert_int_eq(sheet->animations[0].frames[0].rect.w, 9);
    ck_assert_int_eq(sheet->animations[0].frames[0].rect.h, 20);

    ck_assert_int_eq(sheet->animations[0].frames[0].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet->animations[0].frames[0].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet->animations[0].frames[0].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet->animations[0].frames[0].hit_boxes[0]. h, 20);

    //second frame
    ck_assert_int_eq(sheet->animations[0].frames[1].delay, 20);

    ck_assert_int_eq(sheet->animations[0].frames[1].rect.x, 9);
    ck_assert_int_eq(sheet->animations[0].frames[1].rect.y, 0);
    ck_assert_int_eq(sheet->animations[0].frames[1].rect.w, 9);
    ck_assert_int_eq(sheet->animations[0].frames[1].rect.h, 20);

    ck_assert_int_eq(sheet->animations[0].frames[1].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet->animations[0].frames[1].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet->animations[0].frames[1].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet->animations[0].frames[1].hit_boxes[0]. h, 20);

    // second animation 
    // first frame
    ck_assert_int_eq(sheet->animations[1].frames[0].delay, 5);

    ck_assert_int_eq(sheet->animations[1].frames[0].rect.x, 0);
    ck_assert_int_eq(sheet->animations[1].frames[0].rect.y, 20);
    ck_assert_int_eq(sheet->animations[1].frames[0].rect.w, 9);
    ck_assert_int_eq(sheet->animations[1].frames[0].rect.h, 20);

    ck_assert_int_eq(sheet->animations[1].frames[0].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet->animations[1].frames[0].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet->animations[1].frames[0].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet->animations[1].frames[0].hit_boxes[0]. h, 20);

    // second frame
    ck_assert_int_eq(sheet->animations[1].frames[1].delay, 5);

    ck_assert_int_eq(sheet->animations[1].frames[1].rect.x, 9);
    ck_assert_int_eq(sheet->animations[1].frames[1].rect.y, 20);
    ck_assert_int_eq(sheet->animations[1].frames[1].rect.w, 9);
    ck_assert_int_eq(sheet->animations[1].frames[1].rect.h, 20);

    ck_assert_int_eq(sheet->animations[1].frames[1].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet->animations[1].frames[1].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet->animations[1].frames[1].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet->animations[1].frames[1].hit_boxes[0]. h, 20);
}
END_TEST

Suite *import_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("import");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, IMP_read_level_check_positive);
    tcase_add_test(tc_core, IMP_read_level_check_negative);
    tcase_add_test(tc_core, IMP_read_animation_check);

    suite_add_tcase(s, tc_core);

    return s;
}
