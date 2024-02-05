#include <stdio.h>

#include "../source.h"
#include "../global.h"

#include "library.h"

#define DEFAULT_DIFFUSE 40.0

lightsource_t* lighsources_library[LIGHTSOURCE_ALL] = { NULL };

lightsource_t lighsource_wallight = {
    LIGHTSOURCE_WALLLIGHT,
    PI/6, 
    2, 
    {
        (lightpolygon_t){0, 0, 226, 13, 13, 50, 0, 0,  DEFAULT_DIFFUSE},
        (lightpolygon_t){0, 0, 226, 13, 13, 50, 0, PI, DEFAULT_DIFFUSE},
    }
};

lightsource_t lighsource_lighter = {
    LIGHTSOURCE_LIGHTER,
    PI/7, 
    3, 
    {
        (lightpolygon_t){0,   0, 200, 200, 120, 50,  0, 0,  DEFAULT_DIFFUSE * 1.0},
        (lightpolygon_t){0,  -2, 200, 200, 120, 30, 20, 0,  DEFAULT_DIFFUSE * 0.8},
        (lightpolygon_t){0,  +2, 200, 200, 120, 20, 20, 0,  DEFAULT_DIFFUSE * 0.8},
    }
};

lightsource_t lighsource_lamp_small = {
    LIGHTSOURCE_LAMP_SMALL,
    PI/12, 
    6, 
    {
        (lightpolygon_t){0,   0, 57, 255, 20, 50, 0,   0,         DEFAULT_DIFFUSE * 1.8},
        (lightpolygon_t){0,  -2, 57, 255, 20, 30, 20,  0+PI/12,   DEFAULT_DIFFUSE * 1.0},
        (lightpolygon_t){0,  +2, 57, 255, 20, 30, 20,  0-PI/12,   DEFAULT_DIFFUSE * 1.0},
        (lightpolygon_t){0,   0, 57, 255, 20, 50, 0,   PI,        DEFAULT_DIFFUSE * 1.8},
        (lightpolygon_t){0,  -2, 57, 255, 20, 30, 20,  PI+PI/12,  DEFAULT_DIFFUSE * 1.0},
        (lightpolygon_t){0,  +2, 57, 255, 20, 30, 20,  PI-PI/12,  DEFAULT_DIFFUSE * 1.0},
    }
};

lightsource_t lighsource_lamp_big = {
    LIGHTSOURCE_LAMP_BIG,
    PI/12, 
    6, 
    {
        (lightpolygon_t){0,   0, 255, 87, 51, 50, 0,   0,         DEFAULT_DIFFUSE * 1.8},
        (lightpolygon_t){0,  -2, 255, 87, 51, 30, 20,  0+PI/12,   DEFAULT_DIFFUSE * 1.0},
        (lightpolygon_t){0,  +2, 255, 87, 51, 30, 20,  0-PI/12,   DEFAULT_DIFFUSE * 1.0},
        (lightpolygon_t){0,   0, 255, 87, 51, 50, 0,   PI,        DEFAULT_DIFFUSE * 1.8},
        (lightpolygon_t){0,  -2, 255, 87, 51, 30, 20,  PI+PI/12,  DEFAULT_DIFFUSE * 1.0},
        (lightpolygon_t){0,  +2, 255, 87, 51, 30, 20,  PI-PI/12,  DEFAULT_DIFFUSE * 1.0},
    }
};

lightsource_t lighsource_no = { };

void LIB_read_lightsource_blueprint(
    lightsource_t * blueprint
) {
    if (blueprint) {
        printf("LIGHTSOURCE BLUEPRINT ID = %d read\n", blueprint->id);
    }
    lighsources_library[blueprint->id]      = blueprint;
};

void LIB_create_lightsources_library(
) {
    LIB_read_lightsource_blueprint(&lighsource_lighter);
    LIB_read_lightsource_blueprint(&lighsource_wallight);
    LIB_read_lightsource_blueprint(&lighsource_no);
    LIB_read_lightsource_blueprint(&lighsource_lamp_small);
    LIB_read_lightsource_blueprint(&lighsource_lamp_big);
}
