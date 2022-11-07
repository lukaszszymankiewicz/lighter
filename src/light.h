#include "assets.h"
#include "global.h"
#include "primitives.h"
#include "tile.h"

#ifndef LIGHT_H
#define LIGHT_H

#define R                               2000
#define RAY_THRESHOLD                   2

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
    float             width;
    float             angle;
    int               penetrating_power;
    int               frame;
    int               n_poly;
    int               n_wobbles;
    int               curent_wobble;
    texture_t        *gradient;
    lightpolygon_t   *light_polygons;
    wobble_t         *wobble;
} lightsource_t;

extern lightsource_t     *lightsources[ASSET_LIGHTSOURCE_ALL];
extern wobble_t          *wobbles[ASSET_WOBBLE_ALL];

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
void LIG_free_wobble(wobble_t* wobble);
void LIG_free_lightsource(lightsource_t* lightsource);

void LIG_change_source(lightsource_t *lght);
void LIG_move_lightsource(lightsource_t* light_o, direction_t light_dir, direction_t hero_dir, int frame);
void LIG_fill_lightbuffer(int x, int y, lightsource_t *light, segment_t *obstacles);

void LIG_debug_obstacle(segment_t *obstacles);

lightsource_t* LIG_read_lightsource(const char *filepath);
wobble_t* LIG_read_wobble( const char *filepath);

#endif
