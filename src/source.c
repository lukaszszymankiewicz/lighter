#include <math.h>
#include <stdlib.h>

#include "global.h"
#include "source.h"

#define LIGHT_LEFT   PI/2
#define LIGHT_RIGHT  PI/2*3
#define LIGHT_VER    PI/6

// changes done to light angle if looking in different direction
float lightpos_up_down_corr[2][5] = {
  // LEFT  RIGHT  UP                     DOWN                   NONE
     {0,   0,     LIGHT_RIGHT-LIGHT_VER, LIGHT_RIGHT+LIGHT_VER, LIGHT_RIGHT },      // RIGHT 
     {0,   0,     LIGHT_LEFT +LIGHT_VER, LIGHT_LEFT -LIGHT_VER, LIGHT_LEFT  }       // LEFT 
};

float SRC_move_lightsource(
    direction_t        light_dir,
    direction_t        entity_dir
) {
    return lightpos_up_down_corr[entity_dir][light_dir];
}
int SRC_get_light_polygon_x_corr(
    lightsource_t *source,
    float          angle,
    int            i
) {
    return sin(angle) * source->light_polygons[i].x + cos(angle) * source->light_polygons[i].y;
}

int SRC_get_light_polygon_y_corr(
    lightsource_t *source,
    float          angle,
    int            i 
) {
    return sin(angle) * source->light_polygons[i].y + cos(angle) * source->light_polygons[i].x;
}

float SRC_get_light_polygon_angle_corr(
    lightsource_t *source,
    int            i
) {
    return source->light_polygons[i].angle_corr;
}

float SRC_get_diffuse(
    lightsource_t *source,
    int            i
) {
    return source->light_polygons[i].diffuse;
}

float SRC_get_light_polygon_width_corr(
    lightsource_t *source,
    int            i
) {
    int coef = source->light_polygons[i].width;

    if (coef) {
        return PI / coef;
    }
    return 0.0;
}

void SRC_free_lightsource(
    lightsource_t* source
) {
    free(source);
    source = NULL;
};

