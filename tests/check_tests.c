#include <check.h>
#include <stdlib.h>
#include "../src/events.h"
#include "../src/gfx.h"
#include "../src/tile.h"
#include "../src/segment.h"
#include "../src/sprites.h"
#include "../src/lightpt.h"
#include "../src/geometry.h"
#include "check_events.h"
#include "check_gfx.h"
#include "check_geometry.h"
#include "check_tile.h"
#include "check_segment.h"
#include "check_sprites.h"
#include "check_lightpt.h"


int main(void)
{
    int number_failed = 0;
    Suite *s;
    SRunner *sr;

    // events
    s = events_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // geometry
    s = geometry_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // GFX
    s = gfx_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // lightpt
    s = lightpt_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // tile
    s = tile_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // segment
    s = segment_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // sprites
    s = sprites_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    srunner_free(sr);

    return number_failed == 0 ? 0 : 1;
}

