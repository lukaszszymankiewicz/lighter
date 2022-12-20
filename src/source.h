#include "assets.h"
#include "global.h"
#include "primitives.h"

#ifndef SOURCE_H
#define SOURCE_H

#define LANTERN_WOBBLES_N 2

enum lightsources_names { LIGHTER, LANTERN, ALL };
enum polydata           { X, Y, RED, GREEN, BLUE, LIGHT_POWER, WIDTH };

# define MAX_WOBBLES 2

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
    wobble_t         *wobble[MAX_WOBBLES];
} lightsource_t;

extern lightsource_t     *lightsources[ASSET_LIGHTSOURCE_ALL];
extern wobble_t          *wobbles[ASSET_WOBBLE_ALL];

int SRC_get_light_penetrating_power(lightsource_t *lght);
int SRC_get_light_polygon_x_corr(lightsource_t *lght, int i);
int SRC_get_light_polygon_y_corr(lightsource_t *lght, int i);

float SRC_get_light_polygon_width_corr(lightsource_t *lght, int i);
float SRC_get_wobble_angle_coef(lightsource_t *lght);

lightsource_t* SRC_read_lightsource(const char *filepath);
wobble_t* SRC_read_wobble(const char *filepath);

void SRC_move_lightsource(lightsource_t *light, direction_t light_dir, direction_t entity_dir);
void SRC_change_wobble(lightsource_t *lght, int wobble_index);
void SRC_free_wobble(wobble_t* wobble);
void SRC_free_lightsource(lightsource_t* lightsource);
void SRC_set_wobble(lightsource_t *lght, wobble_t *wobble, int n);

#endif
