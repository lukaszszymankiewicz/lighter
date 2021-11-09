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

    // CLEANUP
    TXTR_free_animation_sheet(sheet);
}
END_TEST

START_TEST (TXTR_init_animation_check)
{
    // GIVEN 
    int xs[] = { 0, 15, 30, 45};
    int ys[] = { 0,  0,  0,  0};
    int ws[] = {15, 15, 15, 15};
    int hs[] = {20, 20, 20, 20};

    int delay = 20;
    int len = 4;

    // WHEN
    animation_t* animation = TXTR_init_animation(xs, ys, ws, hs, delay, len);

    // THEN
    ck_assert_ptr_nonnull(animation);

    ck_assert_int_eq(animation->frames[0].x, 0);
    ck_assert_int_eq(animation->frames[0].y, 0);
    ck_assert_int_eq(animation->frames[0].w, 15);
    ck_assert_int_eq(animation->frames[0].h, 20);

    ck_assert_int_eq(animation->frames[1].x, 15);
    ck_assert_int_eq(animation->frames[1].y, 0);
    ck_assert_int_eq(animation->frames[1].w, 15);
    ck_assert_int_eq(animation->frames[1].h, 20);

    ck_assert_int_eq(animation->frames[2].x, 30);
    ck_assert_int_eq(animation->frames[2].y, 0);
    ck_assert_int_eq(animation->frames[2].w, 15);
    ck_assert_int_eq(animation->frames[2].h, 20);

    ck_assert_int_eq(animation->frames[3].x, 45);
    ck_assert_int_eq(animation->frames[3].y, 0);
    ck_assert_int_eq(animation->frames[3].w, 15);
    ck_assert_int_eq(animation->frames[3].h, 20);

    ck_assert_int_eq(animation->delay, 20);
    ck_assert_int_eq(animation->len, 4);

    // CLEANUP
    TXTR_free_animation(animation);
}
END_TEST


START_TEST (TXTR_push_animation_check)
{
    // GIVEN 
    char* filepath = "../sprites/her2.png";
    enum state {WALKING, STANDING, MAX};

    // WHEN
    animation_sheet_t* sheet = TXTR_init_animation_sheet(filepath, MAX);

    TXTR_push_animation(
        sheet,
        WALKING,
        (int[]){ 0, 15, 30, 45},
        (int[]){ 0,  0,  0,  0},
        (int[]){15, 15, 15, 15},
        (int[]){20, 20, 20, 20},
        20,
        4
    );
    TXTR_push_animation(
        sheet,
        STANDING,
        (int[]){ 0, 15},
        (int[]){20, 20},
        (int[]){15, 15},
        (int[]){20, 20},
        10,
        2
    );

    // THEN
    ck_assert_int_eq(sheet->animations[0].frames[0].x, 0);
    ck_assert_int_eq(sheet->animations[0].frames[1].x, 15);
    ck_assert_int_eq(sheet->animations[0].frames[2].x, 30);
    ck_assert_int_eq(sheet->animations[0].frames[3].x, 45);

    ck_assert_int_eq(sheet->animations[0].frames[0].y, 0);
    ck_assert_int_eq(sheet->animations[0].frames[1].y, 0);
    ck_assert_int_eq(sheet->animations[0].frames[2].y, 0);
    ck_assert_int_eq(sheet->animations[0].frames[3].y, 0);

    ck_assert_int_eq(sheet->animations[0].frames[0].w, 15);
    ck_assert_int_eq(sheet->animations[0].frames[1].w, 15);
    ck_assert_int_eq(sheet->animations[0].frames[2].w, 15);
    ck_assert_int_eq(sheet->animations[0].frames[3].w, 15);

    ck_assert_int_eq(sheet->animations[0].frames[0].h, 20);
    ck_assert_int_eq(sheet->animations[0].frames[1].h, 20);
    ck_assert_int_eq(sheet->animations[0].frames[2].h, 20);
    ck_assert_int_eq(sheet->animations[0].frames[3].h, 20);

    ck_assert_int_eq(sheet->animations[0].delay, 20);
    ck_assert_int_eq(sheet->animations[0].len, 4);

    ck_assert_int_eq(sheet->animations[1].frames[0].x, 0);
    ck_assert_int_eq(sheet->animations[1].frames[1].x, 15);

    ck_assert_int_eq(sheet->animations[1].frames[0].y, 20);
    ck_assert_int_eq(sheet->animations[1].frames[1].y, 20);

    ck_assert_int_eq(sheet->animations[1].frames[0].w, 15);
    ck_assert_int_eq(sheet->animations[1].frames[1].w, 15);

    ck_assert_int_eq(sheet->animations[1].frames[0].h, 20);
    ck_assert_int_eq(sheet->animations[1].frames[1].h, 20);

    ck_assert_int_eq(sheet->animations[1].delay, 10);
    ck_assert_int_eq(sheet->animations[1].len, 2);

    // CLEANUP
    TXTR_free_animation_sheet(sheet);
}
END_TEST

Suite * sprites_suite(void)
    
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
