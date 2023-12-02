#include "global.h"

#ifndef SOURCE_H
#define SOURCE_H

#define MAX_WOBBLES       2
#define MAX_LIGHTPOLYGON  10
#define MAX_WOBBLE_LEN    100

typedef short int direction_t;

typedef struct lightpolygon { 
    int   x;
    int   y;
    int   red;
    int   green;
    int   blue;
    int   light_power;
    int   width;
    float angle_corr;
} lightpolygon_t;

typedef struct wobble {
    int   len;
    float coefs[MAX_WOBBLE_LEN];
} wobble_t;

typedef struct lightsource {
    float             width;
    int               penetrating_power;
    int               n_poly;
    lightpolygon_t    light_polygons[MAX_LIGHTPOLYGON];
} lightsource_t;

int SRC_get_light_penetrating_power(lightsource_t *lght);
int SRC_get_light_polygon_x_corr(lightsource_t *lght, float angle, int i);
int SRC_get_light_polygon_y_corr(lightsource_t *lght, float angle, int i);

float SRC_get_light_polygon_angle_corr(lightsource_t *source, int i);
float SRC_get_light_polygon_width_corr(lightsource_t *lght, int i);
float SRC_move_lightsource(direction_t light_dir, direction_t entity_dir);

void SRC_free_wobble(wobble_t* wobble);
void SRC_free_lightsource(lightsource_t* lightsource);

#endif
