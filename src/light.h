#include "global.h"
#include "primitives.h"
#include "tile.h"

#ifndef LIGHT_H
#define LIGHT_H

#define R                               2000
#define RAY_THRESHOLD                   2
#define ALL_AVAILABLE_LIGHTSOURCES      2

enum lightsources_names { LIGHTER, LANTERN, ALL };
enum polydata           { X, Y, RED, GREEN, BLUE, LIGHT_POWER, WIDTH };

typedef struct lightpolygon { 
    int x;
    int y;
    int red;
    int green;
    int blue;
    int light_power;
    int width;
} lightpolygon_t;

typedef struct wobble {
    int   len;
    float *coefs;
} wobble_t;

typedef struct lightsource {
    float             width;                  // width (in rad) of light cone (0.0 if light takes whole radius)
    int               n_poly;                 // number of light polygon draw
    int               penetrating_power;
    wobble_t         *wobble[2];              // wobble data
    texture_t        *gradient_texture;
    lightpolygon_t   *light_polygons;         // polys correction and colors
} lightsource_t;

typedef struct light {
    lightsource_t *src;           // current light source
    int            src_num;       // id of light source
    float          angle;         // current angle in which light is cast
} light_t;

// INITS
light_t *LIG_init();
void LIG_read_all_files();

// TESTING
vertex_t* LIG_get_base_light_polygon(int x, int y, segment_t *obstacles, point_t* hit_points);
vertex_t* LIG_find_closest_hit(segment_t* inter, int x1, int y1, int x2, int y2);
vertex_t* LIG_get_light_polygon(int x, int y, float width, float angle, segment_t* obstacles);

segment_t* LIG_ray_intersects_multiple( int x1, int y1, int x2, int y2, segment_t *obstacles);
segment_t* LIG_find_closest_hit_segment_ver(segment_t* inter, int r_x1, int r_y1, int r_x2, int r_y2);
segment_t* LIG_find_closest_hit_segment_hor(segment_t* inter, int r_x1, int r_y1, int r_x2, int r_y2);

int LIG_calculate_dull_ray_end(int r_beg, int r_end, int o_beg, int o_end);
bool LIG_ray_intersect(int o_x1, int o_y1, int o_x2, int o_y2, int r_x1, int r_y1, int r_x2, int r_y2);
void LIG_filter_visible_light(int x, int y, segment_t* obstacles, vertex_t* light_polygon, float angle, float width);

point_t* LIG_generate_aux_hit_points(int x1, int y1, int x2, int y2, segment_t* inter);
point_t* LIG_generate_slipover_hit_point(int x1, int y1, int x2, int y2);
point_t* LIG_generate_hit_points(int x, int y, float width, float angle, segment_t* obstacles);

// FREE
void LIG_free_all_files();
void LIG_free(light_t *lght);

void LIG_change_source(light_t *lght);
void LIG_move_lightsource(light_t* light_o, direction_t light_dir, direction_t hero_dir, int frame);
void LIG_fill_lightbuffer(int x, int y, int frame, light_t *light, segment_t *obstacles, int x_vel);

void LIG_debug_obstacle(segment_t *obstacles);

#endif
