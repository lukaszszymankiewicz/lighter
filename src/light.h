#include "tile.h"
#include "sprites.h"

#ifndef LIGHT_H
#define LIGHT_H


enum light_sources {
    LANTERN = 0,
    FLASHLIGHT = 1,
    ALL = 2,
};

enum polydata {
    X = 0,
    Y = 1,
    COLOR = 2,
};

typedef struct lightsource
{
    float width;                  // width (in rad) of light cone (0.0 if light takes whole radius)
    float corr;                   // correction coef used to cast axuilarry ray (in rad)
    short int n_poly;             // number of light polygon draw
    void (*draw_fun)();           // function to use to draw light polygon
    int polys[][3];               // x_correction, y_correction, color
} lightsource_t;

typedef struct light {
    lightsource_t * src;          // current light source
    short int src_num;            // id of light source
    float angle;                  // current angle in which light is cast
} light_t;

// functions
light_t * LIG_init();
void LIG_draw_light_effect(int x, int y, light_t * light_o, tiles_list_t * tiles);
void LIG_change_source(light_t* lght);

#endif
