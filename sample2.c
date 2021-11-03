#include "src/primitives.h"
#include "src/config.h"
#include "src/obstacle.h"
#include "src/light.h"
#include "src/vertex.h"
#include "src/macros.h"
#include "src/point.h"

int main() 
{
    vertex_t   *light_polygon      = NULL;   // here is points which make light polygon is stored
    int x = 160;
    int y = 144;

    // GIVEN
    obstacle_t *obstacles = NULL;
    point_t    *points    = NULL;

    PT_push(&points, 245, 36);

    // OBS_push(&obstacles, 0, 0, 320, 0);
    // OBS_push(&obstacles, 0, 200, 320, 200);
    // OBS_push(&obstacles, 0, 0, 0, 200);
    // OBS_push(&obstacles, 320, 0, 320, 200);

    // OBS_push(&obstacles, -13, -28, -13, 164);
    // OBS_push(&obstacles, 19, -28, 19, 164);
    // OBS_push(&obstacles, -13, -28, 19, -28);
    // OBS_push(&obstacles, -13, 196, 19, 196);
    // OBS_push(&obstacles, 19, 164, 51, 164);
    // OBS_push(&obstacles, 19, 196, 51, 196);
    // OBS_push(&obstacles, 51, 164, 83, 164);
    // OBS_push(&obstacles, 51, 196, 83, 196);
    // OBS_push(&obstacles, 83, 164, 115, 164);
    // OBS_push(&obstacles, 83, 196, 115, 196);
    // OBS_push(&obstacles, 115, 164, 147, 164);
    // OBS_push(&obstacles, 115, 196, 147, 196);
    // OBS_push(&obstacles, 147, 164, 179, 164);
    // OBS_push(&obstacles, 147, 196, 179, 196);
    // OBS_push(&obstacles, 179, 164, 211, 164);
    // OBS_push(&obstacles, 179, 196, 211, 196);
    // OBS_push(&obstacles, 211, 164, 243, 164);
    // OBS_push(&obstacles, 211, 196, 243, 196);
    OBS_push(&obstacles, 243, 4, 243, 36);
    OBS_push(&obstacles, 275, 4, 275, 36);
    OBS_push(&obstacles, 243, 4, 275, 4);
    OBS_push(&obstacles, 243, 36, 275, 36);
    // OBS_push(&obstacles, 243, 164, 275, 164);
    // OBS_push(&obstacles, 243, 196, 275, 196);
    // OBS_push(&obstacles, 275, 164, 307, 164);
    // OBS_push(&obstacles, 275, 196, 307, 196);
    // OBS_push(&obstacles, 307, 164, 339, 164);
    // OBS_push(&obstacles, 307, 196, 339, 196);

    // base points
    point_t* new_point_a = NULL;

    float angle = VRTX_calculate_angle(x, y, 246, 36);

    // first shifted ray
    new_point_a = LIG_closest_intersection_with_obstacle(
        x,
        y,
        246 - sin(angle + smol_angle) * R,
        26 - cos(angle + smol_angle) * R,
        obstacles
    );
    printf("begin: %i, %i \n", x, y);
    printf("hit point: 246, 36 \n");
    printf("end: %i, %i \n\n", new_point_a->x, new_point_a->y);

    printf("%i %i \n", new_point_a->x, new_point_a->y);
}
