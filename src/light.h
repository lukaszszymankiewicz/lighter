#include <stdbool.h>

#include "global.h"
#include "point.h"
#include "segment.h"
#include "source.h"

#ifndef LIGHT_H
#define LIGHT_H

#define R                               2000
#define RAY_THRESHOLD                   2

typedef struct lvertex {
    vertex_t* coords;
    int       x0;
    int       y0;
    int       power;
    int       red;
    int       green;
    int       blue;
} lvertex_t;

void LIG_add_to_scene(
    int x,      int y,
    int x_corr, int y_corr,
    int i,
    float angle, float coef,
    lightsource_t *light,
    segment_t     *obstacles
);

// TESTING
vertex_t* LIG_get_base_light_polygon(int x, int y, segment_t *obstacles, point_t* hit_points);
vertex_t* LIG_find_closest_hit(segment_t* inter, int x1, int y1, int x2, int y2);
vertex_t* LIG_get_light_polygon(int x, int y, float width, float angle, segment_t* obstacles);

segment_t* LIG_ray_intersects_multiple( int x1, int y1, int x2, int y2, segment_t *obstacles);
segment_t* LIG_find_closest_hit_segment_ver(segment_t* inter, int r_x1, int r_y1, int r_x2, int r_y2);
segment_t* LIG_find_closest_hit_segment_hor(segment_t* inter, int r_x1, int r_y1, int r_x2, int r_y2);

int LIG_calculate_dull_ray_end(int r_beg, int r_end, int o_beg, int o_end);

bool LIG_ray_intersect(int o_x1, int o_y1, int o_x2, int o_y2, int r_x1, int r_y1, int r_x2, int r_y2);

point_t* LIG_generate_aux_hit_points(int x1, int y1, int x2, int y2, segment_t* inter);
point_t* LIG_generate_slipover_hit_point(int x1, int y1, int x2, int y2);
point_t* LIG_generate_hit_points(int x, int y, float width, float angle, segment_t* obstacles);

#endif
