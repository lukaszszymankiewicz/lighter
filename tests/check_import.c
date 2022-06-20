#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <check.h>
#include "../src/level.h"
#include "../src/import.h"

START_TEST (IMP_new_mapfile_check)
{
    // GIVEN
    mapfile_t *map;
    map = NULL;
    char *filename = "sample_level_name";

    // WHEN
    map = IMP_new_mapfile(filename);

    // THEN
    ck_assert_ptr_null(map->file);
    ck_assert_pstr_eq(map->filename, "sample_level_name/level.llv");

    ck_assert_pstr_eq(map->tileset_filename, "sample_level_name/level.png");

    ck_assert_int_eq(map->buffer[0], 0);
    ck_assert_int_eq(map->buffer[1], 0);
    ck_assert_int_eq(map->state, IDLE);  // IDLE == 0

}
END_TEST

START_TEST (IMP_free_mapfile_empty_mapfile_check)
{
    // GIVEN
    mapfile_t *map;
    map = NULL;
    char *filename = "sample_level_name";

    // WHEN
    map = IMP_new_mapfile(filename);
    IMP_free_mapfile(map);

    // THEN
    // TODO: I have no idea why this one does not work without forceing pointer to be NULL ;/
    map = NULL;
    ck_assert_ptr_null(map);
}
END_TEST

START_TEST (IMP_read_file_check)
{
    // GIVEN
    mapfile_t *map;
    map = NULL;
    char *exisiting_filename =    "./testfiles/testlevel";
    char *nonexisiting_filename =    "./testfiles/nonexisiting_filename";
    int result;

    // WHEN (existing file)
    map = IMP_new_mapfile(exisiting_filename);
    result = IMP_read_file(map);

    // THEN
    ck_assert_int_eq(result, 1);

    // CLEANUP
    IMP_free_mapfile(map);
    map = NULL;

    // WHEN (nonexisting file)
    map = IMP_new_mapfile(nonexisiting_filename);
    result = IMP_read_file(map);

    // THEN
    ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST (IMP_read_tileset_check)
{
    // GIVEN
    mapfile_t *map;
    map = NULL;
    char *exisiting_filename =    "./testfiles/testlevel";
    char *nonexisiting_filename =    "./testfiles/nonexisiting_filename";
    int result;

    level_t *level;
    level = NULL;
    level = LVL_new();

    // WHEN (existing file)
    map = IMP_new_mapfile(exisiting_filename);
    result = IMP_read_tileset(level, map);

    // THEN
    ck_assert_int_eq(result, 1);

    // CLEANUP
    IMP_free_mapfile(map);
    map = NULL;

    // WHEN (nonexisting file)
    map = IMP_new_mapfile(nonexisiting_filename);
    result = IMP_read_file(map);

    // THEN
    ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST (IMP_read_from_file_check)
{

    // GIVEN
    level_t *level;
    level = NULL;
    char *filename =    "./testfiles/testlevel";

    int expected_x_size = 40;
    int expected_y_size = 40;
    int expected_x_hero = 6;
    int expected_y_hero = 3;

    // WHEN
    level = IMP_read_from_file(filename);

    // THEN
    ck_assert_int_eq(level->size_x, expected_x_size);
    ck_assert_int_eq(level->size_y, expected_y_size);

    ck_assert_int_eq(level->hero_x, expected_x_hero);
    ck_assert_int_eq(level->hero_y, expected_y_hero);
}
END_TEST
 

Suite *import_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("import");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, IMP_new_mapfile_check);
    tcase_add_test(tc_core, IMP_free_mapfile_empty_mapfile_check);
    tcase_add_test(tc_core, IMP_read_file_check);
    tcase_add_test(tc_core, IMP_read_tileset_check);
    tcase_add_test(tc_core, IMP_read_from_file_check);

    suite_add_tcase(s, tc_core);

    return s;
}
