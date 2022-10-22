#include <check.h>
#include "check_vertex.h"
#include "check_light.h"
#include "check_segment.h"
#include "check_point.h"
#include "check_gfx.h"
#include "check_level.h"
#include "check_import.h"
#include "check_events.h"
#include "check_geometry.h"
#include "check_hero.h"
#include "check_sorted_list.h"
#include "check_tests.h"
#include "../src/vertex.h"
#include "../src/sprites.h"
#include "../src/sorted_list.h"
#include "../src/geometry.h"
#include "../src/light.h"
#include "../src/segment.h"
#include "../src/gfx.h"
#include "../src/import.h"
#include "../src/level.h"
#include "../src/events.h"
#include "../src/hero.h"

int main(void) {
    int number_failed = 0;
    Suite *s;
    SRunner *sr;

    // vertex
    s = vertex_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // light
    s = light_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // segment
    s = segment_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // point
    s = point_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // GFX
    // s = gfx_suite();
    // sr = srunner_create(s);
    // srunner_run_all(sr, CK_ENV);
    // number_failed += srunner_ntests_failed(sr);

    // level
    s = level_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // TODO: (LG-9)
    // events
    // s = events_suite();
    // sr = srunner_create(s);
    // srunner_run_all(sr, CK_ENV);
    // number_failed += srunner_ntests_failed(sr);

    // geometry
    s = geometry_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // hero
    s = hero_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // intersection
    s = sorted_list_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    // import
    s = import_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    srunner_free(sr);
    printf("/n");

    return 0;
}

