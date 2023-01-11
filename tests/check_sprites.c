#include <check.h>
#include "../src/import.h"
#include "../src/assets.h"
#include "../src/sprites.h"
#include "../src/files.h"
#include "../src/gfx.h"


START_TEST (ANIM_read_animation_check)
{
    // GIVEN
    animation_sheet_t *sheet = NULL;
    const char *data               = FILEPATH_HERO_ANIMATION;
    const char *texture            = FILEPATH_SPRITE_HERO;

    // WHEN
    sheet = ANIM_read_animation(data);
    sheet->texture = GFX_read_texture(texture);

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
    ck_assert_int_eq(sheet->animations[0].frames[0].handle_x, 9);
    ck_assert_int_eq(sheet->animations[0].frames[0].handle_y, 10);

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
    ck_assert_int_eq(sheet->animations[0].frames[1].handle_x, 9);
    ck_assert_int_eq(sheet->animations[0].frames[1].handle_y, 10);

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
    ck_assert_int_eq(sheet->animations[1].frames[0].handle_x, 9);
    ck_assert_int_eq(sheet->animations[1].frames[0].handle_y, 10);

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
    ck_assert_int_eq(sheet->animations[1].frames[1].handle_x, 9);
    ck_assert_int_eq(sheet->animations[1].frames[1].handle_y, 10);

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
    ck_assert_int_eq(sheet->animations[2].frames[0].handle_x, 9);
    ck_assert_int_eq(sheet->animations[2].frames[0].handle_y, 10);

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
    ck_assert_int_eq(sheet->animations[3].frames[0].handle_x, 9);
    ck_assert_int_eq(sheet->animations[3].frames[0].handle_y, 10);

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

Suite *sprites_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("sprites");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, ANIM_read_animation_check);

    suite_add_tcase(s, tc_core);

    return s;
}
