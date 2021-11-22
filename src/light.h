#include "def.h"
#include "config.h"
#include "tile.h"
#include "primitives.h"

#ifndef LIGHT_H
#define LIGHT_H


#define R          1200
#define smol_angle 0.01
#define eps_angle  0.1

// this is used as column numbers in polys matrix (lightsource_t) - first column
// determines x offset for given poly, second: y-offset and third: color of
// given lightpoly.
enum wobbles            { WOBBLE_DOWN = -1, WOBBLE_UP = 1};
enum lightsources_names { LIGHTER, LANTERN, ALL };
enum polydata           { X, Y, RED, GREEN, BLUE, ALPHA, WIDTH };

typedef struct wobble {
    int   len;
    float coefs[];
} wobble_t;

typedef struct lightsource {
    float    width;                  // width (in rad) of light cone (0.0 if light takes whole radius)
    int      n_poly;                 // number of light polygon draw
    wobble_t *wobble[2];              // wobble data
    int      polys[][7];             // polys correction and colors
} lightsource_t;

typedef struct light {
    lightsource_t *src;           // current light source
    int            src_num;       // id of light source
    float          angle;         // current angle in which light is cast
} light_t;

light_t *LIG_init();
void LIG_change_source(light_t *lght);
void LIG_free(light_t *lght);
void LIG_move_lightsource(light_t* light_o, direction_t light_dir, direction_t hero_dir, int frame);
void LIG_draw_light_effect(int x, int y, int frame, light_t *light, segment_t *obstacles, int x_vel);

// only for testing
vertex_t* LIG_calc_light_polygon(int x, int y, float angle, float width, segment_t *obstacles);
point_t* LIG_calc_hit_points(int x, int y, float angle, float width, segment_t *obstacles);
point_t* LIG_ray_hits_obstacle(int x1, int y1, int x2, int y2, segment_t *obstacle);
point_t* LIG_closest_intersection_with_obstacle(int x1, int y1, int x2, int y2, segment_t *obstacles);
bool LIG_any_intersection_with_obstacle(int x1, int y1, int x2, int y2, segment_t *obstacles);

#endif
