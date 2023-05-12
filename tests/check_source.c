#include <check.h>

#include "../src/data/library.h"
#include "../src/files.h"
#include "../src/source.h"

START_TEST (SRC_read_wobble_check)
{
    // GIVEN
    LIB_create_wobble_library();
    wobble_t *wobble     = NULL;

    // WHEN
    wobble = wobble_library[WOBBLE_WALKING];

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

START_TEST (SRC_read_lightsource_lighter_check)
{
    // GIVEN
    lightsource_t *lightsource  = NULL;
    const char *filename        = FILEPATH_LIGTHER_LIGHTSOURCE;

    // WHEN
    lightsource = SRC_read_lightsource(filename);

    // THEN
    ck_assert_ptr_nonnull(lightsource);

    ck_assert_float_eq(lightsource->width, PI / 7);
    ck_assert_int_eq(lightsource->penetrating_power, 7);
    ck_assert_int_eq(lightsource->n_poly, 3);

    ck_assert_int_eq(lightsource->light_polygons[0].x, 0);
    ck_assert_int_eq(lightsource->light_polygons[0].y, 0);
    ck_assert_int_eq(lightsource->light_polygons[0].red, 255);
    ck_assert_int_eq(lightsource->light_polygons[0].green, 251);
    ck_assert_int_eq(lightsource->light_polygons[0].blue, 187);
    ck_assert_int_eq(lightsource->light_polygons[0].light_power, 50);
    ck_assert_int_eq(lightsource->light_polygons[0].width, 0);

    ck_assert_int_eq(lightsource->light_polygons[1].x, 0);
    ck_assert_int_eq(lightsource->light_polygons[1].y, 0);
    ck_assert_int_eq(lightsource->light_polygons[1].red, 255);
    ck_assert_int_eq(lightsource->light_polygons[1].green, 251);
    ck_assert_int_eq(lightsource->light_polygons[1].blue, 187);
    ck_assert_int_eq(lightsource->light_polygons[1].light_power, 30);
    ck_assert_int_eq(lightsource->light_polygons[1].width, 36);

    ck_assert_int_eq(lightsource->light_polygons[2].x, 0);
    ck_assert_int_eq(lightsource->light_polygons[2].y, 0);
    ck_assert_int_eq(lightsource->light_polygons[2].red, 255);
    ck_assert_int_eq(lightsource->light_polygons[2].green, 251);
    ck_assert_int_eq(lightsource->light_polygons[2].blue, 187);
    ck_assert_int_eq(lightsource->light_polygons[2].light_power, 20);
    ck_assert_int_eq(lightsource->light_polygons[2].width, 72);
}
END_TEST

START_TEST (SRC_read_lightsource_lantern_check)
{
    // GIVEN
    lightsource_t *lightsource = NULL;
    const char *filename       = FILEPATH_WALLLIGHT_LIGHTSOURCE;

    // WHEN
    lightsource = SRC_read_lightsource(filename);

    // THEN
    ck_assert_ptr_nonnull(lightsource);

    ck_assert_float_eq(lightsource->width, 0.0);
    ck_assert_int_eq(lightsource->penetrating_power, 7);
    ck_assert_int_eq(lightsource->n_poly, 5);

    ck_assert_int_eq(lightsource->light_polygons[0].x, -10);
    ck_assert_int_eq(lightsource->light_polygons[0].y, -10);
    ck_assert_int_eq(lightsource->light_polygons[0].red, 255);
    ck_assert_int_eq(lightsource->light_polygons[0].green, 251);
    ck_assert_int_eq(lightsource->light_polygons[0].blue, 187);
    ck_assert_int_eq(lightsource->light_polygons[0].light_power, 10);
    ck_assert_int_eq(lightsource->light_polygons[0].width, 0);

    ck_assert_int_eq(lightsource->light_polygons[1].x, 10);
    ck_assert_int_eq(lightsource->light_polygons[1].y, -10);
    ck_assert_int_eq(lightsource->light_polygons[1].red, 255);
    ck_assert_int_eq(lightsource->light_polygons[1].green, 251);
    ck_assert_int_eq(lightsource->light_polygons[1].blue, 187);
    ck_assert_int_eq(lightsource->light_polygons[1].light_power, 10);
    ck_assert_int_eq(lightsource->light_polygons[1].width, 0);

    ck_assert_int_eq(lightsource->light_polygons[2].x, 10);
    ck_assert_int_eq(lightsource->light_polygons[2].y, 10);
    ck_assert_int_eq(lightsource->light_polygons[2].red, 255);
    ck_assert_int_eq(lightsource->light_polygons[2].green, 251);
    ck_assert_int_eq(lightsource->light_polygons[2].blue, 187);
    ck_assert_int_eq(lightsource->light_polygons[2].light_power, 10);
    ck_assert_int_eq(lightsource->light_polygons[2].width, 0);

    ck_assert_int_eq(lightsource->light_polygons[3].x, -10);
    ck_assert_int_eq(lightsource->light_polygons[3].y, 10);
    ck_assert_int_eq(lightsource->light_polygons[3].red, 255);
    ck_assert_int_eq(lightsource->light_polygons[3].green, 251);
    ck_assert_int_eq(lightsource->light_polygons[3].blue, 187);
    ck_assert_int_eq(lightsource->light_polygons[3].light_power, 10);
    ck_assert_int_eq(lightsource->light_polygons[3].width, 0);

    ck_assert_int_eq(lightsource->light_polygons[4].x, 0);
    ck_assert_int_eq(lightsource->light_polygons[4].y, 0);
    ck_assert_int_eq(lightsource->light_polygons[4].red, 255);
    ck_assert_int_eq(lightsource->light_polygons[4].green, 251);
    ck_assert_int_eq(lightsource->light_polygons[4].blue, 187);
    ck_assert_int_eq(lightsource->light_polygons[4].light_power, 50);
    ck_assert_int_eq(lightsource->light_polygons[4].width, 0);

}
END_TEST

Suite *source_suite(void) {
    Suite *s;
    TCase *tc_core;
    s = suite_create("source");

    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, SRC_read_wobble_check);
    tcase_add_test(tc_core, SRC_read_lightsource_lighter_check);
    tcase_add_test(tc_core, SRC_read_lightsource_lantern_check);

    suite_add_tcase(s, tc_core);

    return s;
}
