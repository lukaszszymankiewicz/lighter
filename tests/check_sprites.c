#include <check.h>
#include <stdlib.h>
#include "../src/sprites.h"


START_TEST (TXTR_init_animation_sheet_check)
{
    // GIVEN 
    char* filepath = "../sprites/her2.png";
    int n = 2;

    // WHEN
    animation_sheet_t* sheet = TXTR_init_animation_sheet(filepath, n);

    // THEN
    ck_assert_ptr_nonnull(sheet);
}
END_TEST

START_TEST (TXTR_init_animation_check)
{
    // GIVEN 
    int rects[][4] = {
        { 0, 0, 15, 20},
        {15, 0, 15, 20},
        {30, 0, 15, 20},
        {45, 0, 15, 20},
    };

    int hit_boxes[][4] = {
        { 0, 0, 9, 20},  // 0 
        { 0, 0, 9, 20},  // 1 
        { 2, 2, 7, 10},  // 2 - 0
        { 0, 0, 9, 20},  // 2 - 1
        { 2, 2, 7, 10},  // 3 - 0
        { 0, 0, 9, 20},  // 3 - 1
    };

    int delay = 20;
    int n_frames = 4;
    int hit_boxes_len[] = {1, 1, 2, 2};

    // WHEN
    animation_t* animation = TXTR_init_animation(rects, hit_boxes, delay, n_frames, hit_boxes_len);

    // THEN
    ck_assert_ptr_nonnull(animation);

    // frames
    ck_assert_int_eq(animation->frames[0].rect.x, 0);
    ck_assert_int_eq(animation->frames[0].rect.y, 0);
    ck_assert_int_eq(animation->frames[0].rect.w, 15);
    ck_assert_int_eq(animation->frames[0].rect.h, 20);

    ck_assert_int_eq(animation->frames[1].rect.x, 15);
    ck_assert_int_eq(animation->frames[1].rect.y, 0);
    ck_assert_int_eq(animation->frames[1].rect.w, 15);
    ck_assert_int_eq(animation->frames[1].rect.h, 20);

    ck_assert_int_eq(animation->frames[2].rect.x, 30);
    ck_assert_int_eq(animation->frames[2].rect.y, 0);
    ck_assert_int_eq(animation->frames[2].rect.w, 15);
    ck_assert_int_eq(animation->frames[2].rect.h, 20);

    ck_assert_int_eq(animation->frames[3].rect.x, 45);
    ck_assert_int_eq(animation->frames[3].rect.y, 0);
    ck_assert_int_eq(animation->frames[3].rect.w, 15);
    ck_assert_int_eq(animation->frames[3].rect.h, 20);

    ck_assert_int_eq(animation->delay, 20);
    ck_assert_int_eq(animation->len, 4);

    // hitboxs
    ck_assert_int_eq(animation->frames[0].hit_boxes[0].x, 0);
    ck_assert_int_eq(animation->frames[0].hit_boxes[0].y, 0);
    ck_assert_int_eq(animation->frames[0].hit_boxes[0].w, 9);
    ck_assert_int_eq(animation->frames[0].hit_boxes[0].h, 20);

    ck_assert_int_eq(animation->frames[1].hit_boxes[0].x, 0);
    ck_assert_int_eq(animation->frames[1].hit_boxes[0].y, 0);
    ck_assert_int_eq(animation->frames[1].hit_boxes[0].w, 9);
    ck_assert_int_eq(animation->frames[1].hit_boxes[0].h, 20);

    ck_assert_int_eq(animation->frames[2].hit_boxes[0].x, 2);
    ck_assert_int_eq(animation->frames[2].hit_boxes[0].y, 2);
    ck_assert_int_eq(animation->frames[2].hit_boxes[0].w, 7);
    ck_assert_int_eq(animation->frames[2].hit_boxes[0].h, 10);

    ck_assert_int_eq(animation->frames[2].hit_boxes[1].x, 0);
    ck_assert_int_eq(animation->frames[2].hit_boxes[1].y, 0);
    ck_assert_int_eq(animation->frames[2].hit_boxes[1].w, 9);
    ck_assert_int_eq(animation->frames[2].hit_boxes[1].h, 20);

    ck_assert_int_eq(animation->frames[3].hit_boxes[0].x, 2);
    ck_assert_int_eq(animation->frames[3].hit_boxes[0].y, 2);
    ck_assert_int_eq(animation->frames[3].hit_boxes[0].w, 7);
    ck_assert_int_eq(animation->frames[3].hit_boxes[0].h, 10);

    ck_assert_int_eq(animation->frames[3].hit_boxes[1].x, 0);
    ck_assert_int_eq(animation->frames[3].hit_boxes[1].y, 0);
    ck_assert_int_eq(animation->frames[3].hit_boxes[1].w, 9);
    ck_assert_int_eq(animation->frames[3].hit_boxes[1].h, 20);

    // CLEANUP
    free(animation);
}
END_TEST


START_TEST (TXTR_push_animation_check)
{
    // GIVEN 
    char* filepath = "../sprites/her2.png";
    enum state { WALKING, STANDING, MAX };

    // WHEN
    animation_sheet_t* sheet = TXTR_init_animation_sheet(filepath, MAX);

    // FIRST
    int rects_a[][4] = {
        { 0, 0, 15, 20},
        {15, 0, 15, 20},
    };

    int hit_boxes_a[][4] = {
        { 0, 0, 9, 20},  // 0 
        { 0, 0, 9, 20},  // 1 
    };

    int delay_a = 10;
    int n_frames_a = 2;
    int hit_boxes_len_a[] = {1, 1};

    TXTR_push_animation (
        sheet,
        WALKING,
        rects_a,
        hit_boxes_a,
        delay_a,
        n_frames_a,
        hit_boxes_len_a
    );

    // SECOND
    int rects_b[][4] = {
        { 0, 0, 15, 20},
    };

    int hit_boxes_b[][4] = {
        { 0, 0, 9, 20},  // 0 
        { 0, 0, 9, 20},  // 1 
    };

    int delay_b = 10;
    int n_frames_b = 1;
    int hit_boxes_len_b[] = {2};

    TXTR_push_animation (
        sheet,
        STANDING,
        rects_b,
        hit_boxes_b,
        delay_b,
        n_frames_b,
        hit_boxes_len_b
    );


    // THEN
    // FIRST
    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].rect.x, 0);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].rect.y, 0);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].rect.w, 15);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].rect.h, 20);

    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].rect.x, 15);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].rect.y, 0);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].rect.w, 15);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].rect.h, 20);

    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].n_hit_box, 1);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].n_hit_box, 1);

    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].hit_boxes[0].x, 0);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].hit_boxes[0].y, 0);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].hit_boxes[0].w, 9);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[0].hit_boxes[0].h, 20);

    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].hit_boxes[0].x, 0);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].hit_boxes[0].y, 0);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].hit_boxes[0].w, 9);
    ck_assert_int_eq(sheet->animations[WALKING]->frames[1].hit_boxes[0].h, 20);

    ck_assert_int_eq(sheet->animations[WALKING]->delay, 10);
    ck_assert_int_eq(sheet->animations[WALKING]->len, 2);

    // SECOND
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].rect.x, 0);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].rect.y, 0);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].rect.w, 15);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].rect.h, 20);

    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].n_hit_box, 2);

    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].hit_boxes[0].x, 0);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].hit_boxes[0].y, 0);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].hit_boxes[0].w, 9);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].hit_boxes[0].h, 20);

    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].hit_boxes[0].x, 0);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].hit_boxes[0].y, 0);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].hit_boxes[0].w, 9);
    ck_assert_int_eq(sheet->animations[STANDING]->frames[0].hit_boxes[0].h, 20);

    ck_assert_int_eq(sheet->animations[STANDING]->delay, 10);
    ck_assert_int_eq(sheet->animations[STANDING]->len, 1);

    // CLEANUP
    TXTR_free_animation_sheet(sheet);
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
    tcase_add_test(tc_core, TXTR_init_animation_check);
    tcase_add_test(tc_core, TXTR_push_animation_check);

    suite_add_tcase(s, tc_core);

    return s;
}

