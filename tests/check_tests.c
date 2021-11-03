#include <check.h>
#include "check_vertex.h"
#include "check_light.h"
#include "check_obstacle.h"
#include "check_point.h"
#include "../src/vertex.h"
#include "../src/light.h"
#include "../src/obstacle.h"

int main(void) {
    int number_failed = 0;
    Suite *s;
    SRunner *sr;

    printf("\n");

    // vertex
    s = vertex_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    printf("\n");

    // light
    s = light_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    printf("\n");

    // obstacle
    s = obstacle_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    printf("\n");

    // light
    s = point_suite();
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed += srunner_ntests_failed(sr);

    srunner_free(sr);

    return number_failed == 0 ? 0 : 1;
}

