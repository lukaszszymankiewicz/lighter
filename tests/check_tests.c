#include <check.h>

#include "check_animation.h"
#include "check_controller.h"
#include "check_entity.h"
#include "check_entity_manager.h"
#include "check_game.h"
#include "check_geometry.h"
#include "check_level.h"
#include "check_library.h"
#include "check_light.h"
#include "check_point.h"
#include "check_segment.h"
#include "check_sorted_list.h"
#include "check_source.h"
#include "check_tests.h"
#include "check_vertex.h"

#include "../src/vertex.h"
#include "../src/animation.h"
#include "../src/sorted_list.h"
#include "../src/game.h"
#include "../src/geometry.h"
#include "../src/light.h"
#include "../src/segment.h"
#include "../src/gfx.h"
#include "../src/level.h"
#include "../src/controller.h"
#include "../src/entity.h"
#include "../src/entity_manager.h"
#include "../src/source.h"

#include "../src/data/library.h"

int main(void) {
    int number_failed = 0;
    int number_run = 0;

    Suite   *s;
    SRunner *sr;
    
    Suite *suites[] = {
        animation_suite(),
        controller_suite(),
        entity_manager_suite(),
        entity_suite(),
        geometry_suite(),
        game_suite(),
        level_suite(),
        library_suite(),
        light_suite(),
        point_suite(),
        segment_suite(),
        sorted_list_suite(),
        source_suite(),
        vertex_suite(),
    };
    int n_suites = 14;

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

