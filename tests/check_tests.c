#include <check.h>
#include "check_vertex.h"
#include "check_light.h"
#include "check_segment.h"
#include "check_point.h"
#include "check_gfx.h"
#include "check_level.h"
#include "check_sprites.h"
#include "check_controller.h"
#include "check_geometry.h"
#include "check_entity.h"
#include "check_entity_manager.h"
#include "check_sorted_list.h"
#include "check_tests.h"
#include "check_source.h"
#include "../src/vertex.h"
#include "../src/sprites.h"
#include "../src/sorted_list.h"
#include "../src/geometry.h"
#include "../src/light.h"
#include "../src/segment.h"
#include "../src/gfx.h"
#include "../src/import.h"
#include "../src/level.h"
#include "../src/controller.h"
#include "../src/entity.h"
#include "../src/entity_manager.h"
#include "../src/source.h"

int main(void) {
    int number_failed = 0;
    int number_run = 0;

    Suite   *s;
    SRunner *sr;
    
    Suite *suites[] = {
        vertex_suite(),
        light_suite(),
        segment_suite(),
        point_suite(),
        // gfx_suite(), // silented by now
        level_suite(),
        controller_suite(),
        geometry_suite(),
        entity_suite(),
        entity_manager_suite(),
        sorted_list_suite(),
        sprites_suite(),
        source_suite(),
    };
    int n_suites = 12;

    for (int i=0; i<n_suites; i++) {
        s = suites[i];
        sr = srunner_create(s);
        srunner_run_all(sr, CK_ENV);
        number_run += srunner_ntests_run(sr);
        number_failed += srunner_ntests_failed(sr);
    }

    printf("\nOVERVIEW:\n");
    srunner_free(sr);
    printf("\x1B[32mTests passed = %d \x1B[0m\n", number_run);
    printf("\x1B[31mTests failed = %d \x1B[0m\n", number_failed);

    return 0;
}

