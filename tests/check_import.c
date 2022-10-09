#include <check.h>
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
    const char *data               = FILEPATH_HERO_ANIMATION;
    const char *texture            = FILEPATH_SPRITE_HERO;

    // WHEN
    sheet = IMP_read_animation(data);
    sheet->texture = IMP_read_texture(texture);

    // THEN
    ck_assert_ptr_nonnull(sheet);
    ck_assert_ptr_nonnull(sheet->texture);

    // sheet
    ck_assert_int_eq(sheet->n_animations, 4);

    // single animation
    ck_assert_int_eq(sheet->animations[0].len, 2);
    ck_assert_int_eq(sheet->animations[1].len, 2);
    ck_assert_int_eq(sheet->animations[2].len, 1);
    ck_assert_int_eq(sheet->animations[3].len, 1);

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

    // third animation 
    // first frame
    ck_assert_int_eq(sheet->animations[2].frames[0].delay, 0);

    ck_assert_int_eq(sheet->animations[2].frames[0].rect.x, 18);
    ck_assert_int_eq(sheet->animations[2].frames[0].rect.y, 0);
    ck_assert_int_eq(sheet->animations[2].frames[0].rect.w, 9);
    ck_assert_int_eq(sheet->animations[2].frames[0].rect.h, 20);

    ck_assert_int_eq(sheet->animations[2].frames[0].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet->animations[2].frames[0].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet->animations[2].frames[0].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet->animations[2].frames[0].hit_boxes[0]. h, 20);

    // fourth animation 
    // first frame
    ck_assert_int_eq(sheet->animations[3].frames[0].delay, 0);

    ck_assert_int_eq(sheet->animations[3].frames[0].rect.x, 18);
    ck_assert_int_eq(sheet->animations[3].frames[0].rect.y, 20);
    ck_assert_int_eq(sheet->animations[3].frames[0].rect.w, 9);
    ck_assert_int_eq(sheet->animations[3].frames[0].rect.h, 20);

    ck_assert_int_eq(sheet->animations[3].frames[0].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet->animations[3].frames[0].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet->animations[3].frames[0].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet->animations[3].frames[0].hit_boxes[0]. h, 20);
}
END_TEST

START_TEST (IMP_read_wobble_check)
{
    // GIVEN
    wobble_t *wobble = NULL;
    char *filename   = "./assets/wobbles/walking.wbl";

    // WHEN
    wobble = IMP_read_wobble(filename);

    // THEN
    ck_assert_ptr_nonnull(wobble);
    ck_assert_int_eq(wobble->len, 15);

    ck_assert_float_eq(wobble->coefs[0], 0.012);
    ck_assert_float_eq(wobble->coefs[1], 0.054);
    ck_assert_float_eq(wobble->coefs[2], 0.082);
    ck_assert_float_eq(wobble->coefs[3], 0.1);
    ck_assert_float_eq(wobble->coefs[4], 0.082);
    ck_assert_float_eq(wobble->coefs[5], 0.054);
    ck_assert_float_eq(wobble->coefs[6], 0.012);
    ck_assert_float_eq(wobble->coefs[7], 0.0);
    ck_assert_float_eq(wobble->coefs[8], -0.012);
    ck_assert_float_eq(wobble->coefs[9], -0.054);
    ck_assert_float_eq(wobble->coefs[10], -0.082);
    ck_assert_float_eq(wobble->coefs[11], -0.1);
    ck_assert_float_eq(wobble->coefs[12], -0.082);
    ck_assert_float_eq(wobble->coefs[13], -0.054);
    ck_assert_float_eq(wobble->coefs[14], -0.012);
}
END_TEST


START_TEST (IMP_read_all_files_check)
{
    // THEN
    IMP_read_all_files();

    // just the sanity checks
    ck_assert_ptr_nonnull(animations[ASSET_HERO_ANIMATION]);
    ck_assert_ptr_nonnull(gradients[ASSET_GRADIENT_CIRCULAR]);
    ck_assert_ptr_nonnull(sprites[ASSET_SPRITE_HERO]);
    ck_assert_ptr_nonnull(wobbles[ASSET_WOBBLE_NO]);
    ck_assert_ptr_nonnull(wobbles[ASSET_WOBBLE_STABLE]);
    ck_assert_ptr_nonnull(wobbles[ASSET_WOBBLE_WALKING]);
    ck_assert_ptr_nonnull(levels[ASSET_LEVEL_SAMPLE]);
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
    tcase_add_test(tc_core, IMP_read_wobble_check);
    tcase_add_test(tc_core, IMP_read_all_files_check);

    suite_add_tcase(s, tc_core);

    return s;
}
