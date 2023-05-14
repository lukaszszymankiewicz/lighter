#include <check.h>

#include "../src/data/library.h"
#include "../src/animation.h"


START_TEST (ANIM_read_animation_check)
{
    // GIVEN
    LIB_create_entity_library();
    animation_sheet_t sheet = entity_library[ENTITY_HERO]->animation;

    // sheet
    ck_assert_int_eq(sheet.n_animations, 4);
    ck_assert_int_eq(sheet.texture_id, 0);
    ck_assert_int_eq(sheet.width, 27);
    ck_assert_int_eq(sheet.height, 20);

    // single animation
    ck_assert_int_eq(sheet.animations[0].len, 2);
    ck_assert_int_eq(sheet.animations[1].len, 2);
    ck_assert_int_eq(sheet.animations[2].len, 1);
    ck_assert_int_eq(sheet.animations[3].len, 1);

    ck_assert_int_eq(sheet.animations[0].wobble_id, 2);
    ck_assert_int_eq(sheet.animations[1].wobble_id, 1);
    ck_assert_int_eq(sheet.animations[2].wobble_id, 0);
    ck_assert_int_eq(sheet.animations[3].wobble_id, 0);

    // // single frame
    // // first animation 
    // // first frame
    ck_assert_int_eq(sheet.animations[0].frames[0].delay, 20);
    ck_assert_int_eq(sheet.animations[0].frames[0].handle_x, 9);
    ck_assert_int_eq(sheet.animations[0].frames[0].handle_y, 10);

    ck_assert_int_eq(sheet.animations[0].frames[0].rect.x, 0);
    ck_assert_int_eq(sheet.animations[0].frames[0].rect.y, 0);
    ck_assert_int_eq(sheet.animations[0].frames[0].rect.w, 9);
    ck_assert_int_eq(sheet.animations[0].frames[0].rect.h, 20);

    ck_assert_int_eq(sheet.animations[0].frames[0].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet.animations[0].frames[0].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet.animations[0].frames[0].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet.animations[0].frames[0].hit_boxes[0]. h, 20);

    //second frame
    ck_assert_int_eq(sheet.animations[0].frames[1].delay, 20);
    ck_assert_int_eq(sheet.animations[0].frames[1].handle_x, 9);
    ck_assert_int_eq(sheet.animations[0].frames[1].handle_y, 10);

    ck_assert_int_eq(sheet.animations[0].frames[1].rect.x, 9);
    ck_assert_int_eq(sheet.animations[0].frames[1].rect.y, 0);
    ck_assert_int_eq(sheet.animations[0].frames[1].rect.w, 9);
    ck_assert_int_eq(sheet.animations[0].frames[1].rect.h, 20);

    ck_assert_int_eq(sheet.animations[0].frames[1].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet.animations[0].frames[1].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet.animations[0].frames[1].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet.animations[0].frames[1].hit_boxes[0]. h, 20);

    // second animation 
    // first frame
    ck_assert_int_eq(sheet.animations[1].frames[0].delay, 5);
    ck_assert_int_eq(sheet.animations[1].frames[0].handle_x, 9);
    ck_assert_int_eq(sheet.animations[1].frames[0].handle_y, 10);

    ck_assert_int_eq(sheet.animations[1].frames[0].rect.x, 0);
    ck_assert_int_eq(sheet.animations[1].frames[0].rect.y, 20);
    ck_assert_int_eq(sheet.animations[1].frames[0].rect.w, 9);
    ck_assert_int_eq(sheet.animations[1].frames[0].rect.h, 20);

    ck_assert_int_eq(sheet.animations[1].frames[0].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet.animations[1].frames[0].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet.animations[1].frames[0].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet.animations[1].frames[0].hit_boxes[0]. h, 20);

    // second frame
    ck_assert_int_eq(sheet.animations[1].frames[1].delay, 5);
    ck_assert_int_eq(sheet.animations[1].frames[1].handle_x, 9);
    ck_assert_int_eq(sheet.animations[1].frames[1].handle_y, 10);

    ck_assert_int_eq(sheet.animations[1].frames[1].rect.x, 9);
    ck_assert_int_eq(sheet.animations[1].frames[1].rect.y, 20);
    ck_assert_int_eq(sheet.animations[1].frames[1].rect.w, 9);
    ck_assert_int_eq(sheet.animations[1].frames[1].rect.h, 20);

    ck_assert_int_eq(sheet.animations[1].frames[1].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet.animations[1].frames[1].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet.animations[1].frames[1].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet.animations[1].frames[1].hit_boxes[0]. h, 20);

    // third animation 
    // first frame
    ck_assert_int_eq(sheet.animations[2].frames[0].delay, 0);
    ck_assert_int_eq(sheet.animations[2].frames[0].handle_x, 9);
    ck_assert_int_eq(sheet.animations[2].frames[0].handle_y, 10);

    ck_assert_int_eq(sheet.animations[2].frames[0].rect.x, 18);
    ck_assert_int_eq(sheet.animations[2].frames[0].rect.y, 0);
    ck_assert_int_eq(sheet.animations[2].frames[0].rect.w, 9);
    ck_assert_int_eq(sheet.animations[2].frames[0].rect.h, 20);

    ck_assert_int_eq(sheet.animations[2].frames[0].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet.animations[2].frames[0].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet.animations[2].frames[0].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet.animations[2].frames[0].hit_boxes[0]. h, 20);

    // fourth animation 
    // first frame
    ck_assert_int_eq(sheet.animations[3].frames[0].delay, 0);
    ck_assert_int_eq(sheet.animations[3].frames[0].handle_x, 9);
    ck_assert_int_eq(sheet.animations[3].frames[0].handle_y, 10);

    ck_assert_int_eq(sheet.animations[3].frames[0].rect.x, 18);
    ck_assert_int_eq(sheet.animations[3].frames[0].rect.y, 20);
    ck_assert_int_eq(sheet.animations[3].frames[0].rect.w, 9);
    ck_assert_int_eq(sheet.animations[3].frames[0].rect.h, 20);

    ck_assert_int_eq(sheet.animations[3].frames[0].hit_boxes[0]. x, 0);
    ck_assert_int_eq(sheet.animations[3].frames[0].hit_boxes[0]. y, 0);
    ck_assert_int_eq(sheet.animations[3].frames[0].hit_boxes[0]. w, 9);
    ck_assert_int_eq(sheet.animations[3].frames[0].hit_boxes[0]. h, 20);
}
END_TEST

// START_TEST (ANIM_read_animation_check_empty)
// {
//     // GIVEN
//     LIB_create_entity_library();   // fill library
//     animation_sheet_t *sheet       = NULL;
//     int* data                      = entity_library[ENTITY_LIGHTER]->animation;
// 
//     // WHEN
//     sheet = ANIM_read_animation(data, NULL);
// 
//     // THEN
//     ck_assert_int_eq(sheet->n_animations, 0);
//     ck_assert_ptr_nonnull(sheet);
//     ck_assert_ptr_null(sheet->texture);
// 
// }
// END_TEST

Suite *animation_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("animation");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, ANIM_read_animation_check);
    // tcase_add_test(tc_core, ANIM_read_animation_check_empty);

    suite_add_tcase(s, tc_core);

    return s;
}
