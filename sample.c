#include "src/primitives.h"
#include "src/config.h"
#include "src/obstacle.h"
#include "src/light.h"
#include "src/vertex.h"
#include "src/macros.h"

int main() {
    // GIVEN
    obstacle_t *obstacles = NULL;
    point_t    *points    = NULL;

    OBS_push(&obstacles, 0, 0, 320, 0);
    OBS_push(&obstacles, 0, 200, 320, 200);
    OBS_push(&obstacles, 0, 0, 0, 200);
    OBS_push(&obstacles, 320, 0, 320, 200);
    OBS_push(&obstacles, -23, 164, 9, 164);
    OBS_push(&obstacles, -23, 196, 9, 196);
    OBS_push(&obstacles, 9, -28, 9, 164);
    OBS_push(&obstacles, 41, -28, 41, 164);
    OBS_push(&obstacles, 9, -28, 41, -28);
    OBS_push(&obstacles, 9, 196, 41, 196);
    OBS_push(&obstacles, 41, 164, 73, 164);
    OBS_push(&obstacles, 41, 196, 73, 196);
    OBS_push(&obstacles, 73, 164, 105, 164);
    OBS_push(&obstacles, 73, 196, 105, 196);
    OBS_push(&obstacles, 105, 164, 137, 164);
    OBS_push(&obstacles, 105, 196, 137, 196);
    OBS_push(&obstacles, 137, 164, 169, 164);
    OBS_push(&obstacles, 137, 196, 169, 196);
    OBS_push(&obstacles, 169, 164, 201, 164);
    OBS_push(&obstacles, 169, 196, 201, 196);
    OBS_push(&obstacles, 201, 164, 233, 164);
    OBS_push(&obstacles, 201, 196, 233, 196);
    OBS_push(&obstacles, 233, 164, 265, 164);
    OBS_push(&obstacles, 233, 196, 265, 196);
    OBS_push(&obstacles, 265, 4, 265, 36);
    OBS_push(&obstacles, 297, 4, 297, 36);
    OBS_push(&obstacles, 265, 4, 297, 4);
    OBS_push(&obstacles, 265, 36, 297, 36);
    OBS_push(&obstacles, 265, 164, 297, 164);
    OBS_push(&obstacles, 265, 196, 297, 196);
    OBS_push(&obstacles, 297, 164, 329, 164);
    OBS_push(&obstacles, 297, 196, 329, 196);

    int expected_len = 5;

    int x = SCREEN_WIDTH / 2;
    int y = 144;
    float width = 0.448786;
    float angle = -0.837145;

    // THEN
    points = LIG_calc_hit_points(x, y, angle, width, obstacles);
    
    point_t *ptr = NULL;
    ptr          = points;

    obstacle_t *filtered_obstacles = NULL;   // not every level obstacle is taken into account
    vertex_t   *light_polygon      = NULL;   // here is points which make light polygon is stored
    point_t    *hit_points         = NULL;   // here is points to check ray hit stored

    // for each of point to hit n_width_rays rays are casted
    for(point_t* pt=ptr; pt; pt=pt->next) {
        point_t    *new_point          = NULL;
        point_t    *new_point_a        = NULL;
        point_t    *new_point_b        = NULL;

        angle = VRTX_calculate_angle(x, y, pt->x, pt->y);

        // direct ray
        new_point = LIG_closest_intersection_with_obstacle(x, y, pt->x, pt->y, obstacles);
        VRTX_add_point(&light_polygon, new_point->x, new_point->y, angle);

        // first shifted ray
        new_point_a = LIG_closest_intersection_with_obstacle(
            x,
            y,
            pt->x - sin(angle + smol_angle) * R,
            pt->y - cos(angle + smol_angle) * R,
            obstacles
        );
        VRTX_add_point(&light_polygon, new_point_a->x, new_point_a->y, angle+smol_angle);

        // second shifted ray
        new_point_b = LIG_closest_intersection_with_obstacle(
            x,
            y,
            pt->x - sin(angle - smol_angle) * R,
            pt->y - cos(angle - smol_angle) * R,
            obstacles
        );
        VRTX_add_point(&light_polygon, new_point_b->x, new_point_b->y, angle-smol_angle);
    }

    VRTX_add_point(&light_polygon, x, y, 0);

    vertex_t* ptr2 = NULL;
    ptr2 = light_polygon;

    printf("hero pos: %i %i \n", x, y);
    printf("vertices: \n");
    while (ptr2) {
        printf("%i %i \n", ptr2->x, ptr2->y);
        ptr2=ptr2->next;
    }
    printf("\n\n");

}
