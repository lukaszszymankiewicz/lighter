#include "def.h"
#include "config.h"
#include "tile.h"

#ifndef LIGHT_H
#define LIGHT_H

// this is used as column numbers in polys matrix (lightsource_t) - first column
// determines x offset for given poly, second: y-offset and third: color of
// given lightpoly.
enum polydata { X, Y, COLOR };
enum light_sources { LANTERN, FLASHLIGHT, ALL };

// lightsources is basically thing which makes any of light
typedef struct lightsource {
  float width;                // width (in rad) of light cone (0.0 if light takes whole radius)
  float wobble_corr;          // angle correction coef making light wobblig, 0 if no wobbling
  int wobble_change_dir_coef; // how many frames must pass to light start woblling into other direction
  short int n_poly;           // number of light polygon draw
  void (*draw_fun)();         // function to use to draw light polygon
  int polys[][3];             // x_correction, y_correction, color
} lightsource_t;

typedef struct light {
  lightsource_t *src;        // current light source
  short int src_num;         // id of light source
  float angle;               // current angle in which light is cast
} light_t;

light_t *LIG_init();
void LIG_change_source(light_t *lght);
void LIG_free(light_t *lght);
void LIG_move_lightsource(light_t* light_o, direction_t light_dir, direction_t hero_dir, int frame);
void LIG_draw_light_effect(int x, int y, light_t* lght, segment_t* obstacles);

#endif
