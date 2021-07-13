#include <check.h>
#include <stdlib.h>
#include "../src/tile.h"


START_TEST (TILE_init_check)
{
    // GIVEN 
    int x1 = 1; int y1 = 2; int x2 = 3; int y2 = 4;
    int r = 100; int g = 200; int b = 150; int a = 50;

    // WHEN
    tile_t * new_tile;
    new_tile = TILE_init(x1, y1, x2, y2, r, g, b, a);

    // THEN
    ck_assert_int_eq(new_tile->x1, 1);
    ck_assert_int_eq(new_tile->y1, 2);
    ck_assert_int_eq(new_tile->x2, 3);
    ck_assert_int_eq(new_tile->y2, 4);
    ck_assert_int_eq(new_tile->r, 100);
    ck_assert_int_eq(new_tile->g, 200);
    ck_assert_int_eq(new_tile->b, 150);
    ck_assert_int_eq(new_tile->a, 50);

    // CLEANUP
    free(new_tile);
}
END_TEST

START_TEST (TILE_push_check)
{
    // GIVEN 
    int x1 = 1; int y1 = 2; int x2 = 3; int y2 = 4;
    int r1 = 100; int g1 = 200; int b1 = 150; int a1 = 50;

    int x3 = 5; int y3 = 6; int x4 = 7; int y4 = 8;
    int r2 = 10; int g2 = 20; int b2 = 15; int a2 = 5;

    // WHEN
    tiles_list_t * list;
    TILE_push(&list, x1, y1, x2, y2, r1, g1, b1, a1);
    TILE_push(&list, x3, y3, x4, y4, r2, g2, b2, a2);

    // THEN
    ck_assert_int_eq(list->next->tile->x1, 1);
    ck_assert_int_eq(list->next->tile->y1, 2);
    ck_assert_int_eq(list->next->tile->x2, 3);
    ck_assert_int_eq(list->next->tile->y2, 4);
    ck_assert_int_eq(list->next->tile->r, 100);
    ck_assert_int_eq(list->next->tile->g, 200);
    ck_assert_int_eq(list->next->tile->b, 150);
    ck_assert_int_eq(list->next->tile->a, 50);

    ck_assert_int_eq(list->tile->x1, 5);
    ck_assert_int_eq(list->tile->y1, 6);
    ck_assert_int_eq(list->tile->x2, 7);
    ck_assert_int_eq(list->tile->y2, 8);
    ck_assert_int_eq(list->tile->r, 10);
    ck_assert_int_eq(list->tile->g, 20);
    ck_assert_int_eq(list->tile->b, 15);
    ck_assert_int_eq(list->tile->a, 5);

    // CLEANUP
    TILE_free(list);
}
END_TEST


Suite * tile_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("tile");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, TILE_init_check);
    tcase_add_test(tc_core, TILE_push_check);

    suite_add_tcase(s, tc_core);

    return s;
}
