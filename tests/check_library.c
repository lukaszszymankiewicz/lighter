#include <check.h>

#include "../src/data/library.h"

START_TEST (LIB_create_all_check)
{
    // WHEN
    LIB_create_all();
    
    // THEN
    for (int i=0; i<ENTITY_ALL; i++) {
        ck_assert_ptr_nonnull(entity_library[i]);
    }

    for (int i=0; i<LEVEL_ALL; i++) {
        ck_assert_ptr_nonnull(levels_library[i]);
    }

    for (int i=0; i<TILE_ALL; i++) {
        ck_assert_ptr_nonnull(tiles_library[i]);
    }

    for (int i=0; i<LIGHTSOURCE_ALL; i++) {
        ck_assert_ptr_nonnull(lighsources_library[i]);
    }

    for (int i=0; i<SPRITE_ALL; i++) {
        ck_assert_ptr_nonnull(sprites_library[i]);
    }

    for (int i=0; i<TILESET_ALL; i++) {
        ck_assert_ptr_nonnull(tilesets_library[i]);
    }

    for (int i=0; i<WOBBLE_ALL; i++) {
        ck_assert_ptr_nonnull(wobble_library[i]);
    }
}

END_TEST

Suite *library_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("library");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, LIB_create_all_check);

    suite_add_tcase(s, tc_core);

    return s;
}
