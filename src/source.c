#include <math.h>
#include <stdlib.h>

#include "global.h"
#include "import.h"
#include "source.h"
#include "primitives.h"

// changes done to light angle if looking in different direction
float lightpos_up_down_corr[2][5] = {
  // LEFT  RIGHT  UP               DOWN             NONE
     {0,   0,     RIGHT_RAD+DEG30, RIGHT_RAD-DEG30, RIGHT_RAD },      // RIGHT 
     {0,   0,     LEFT_RAD-DEG30,  LEFT_RAD+DEG30,  LEFT_RAD  }       // LEFT 
};

float SRC_move_lightsource(
    direction_t        light_dir,
    direction_t        entity_dir
) {
    return lightpos_up_down_corr[entity_dir][light_dir];
}

int SRC_get_light_penetrating_power(
    lightsource_t *source
) {
    return source->penetrating_power;
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

float SRC_get_light_polygon_width_corr(
    lightsource_t *source,
    int            i
) {
    int coef = source->light_polygons[i].width;

    if (coef){
        return PI / coef;
    }
    return 0.0;
}

void SRC_free_wobble(
    wobble_t* wobble
) {
    free(wobble);
    wobble = NULL;
};

void SRC_free_lightsource(
    lightsource_t* source
) {
    free(source);
    source = NULL;
};

