#include <stdio.h>

#include "../source.h"
#include "../global.h"

#include "library.h"

#define DEFAULT_DIFFUSE 2.5

lightsource_t* lighsources_library[LIGHTSOURCE_ALL] = { NULL };

lightsource_t lighsource_wallight = {
    LIGHTSOURCE_WALLLIGHT,
    PI/6, 7, 2, 
    {
        (lightpolygon_t){0, 0, 226, 13, 13, 50, 0, 0,  DEFAULT_DIFFUSE},
        (lightpolygon_t){0, 0, 226, 13, 13, 50, 0, PI, DEFAULT_DIFFUSE},
    }
};

lightsource_t lighsource_lighter = {
    LIGHTSOURCE_LIGHTER,
    PI/7, 7, 1, 
    {
        (lightpolygon_t){0,  0, 200, 200, 120, 50,  0, 0, DEFAULT_DIFFUSE / 1.5},
        (lightpolygon_t){0, -2, 200, 200, 120, 30, 20, 0, DEFAULT_DIFFUSE},
        (lightpolygon_t){0, +2, 200, 200, 120, 20, 20, 0, DEFAULT_DIFFUSE},
    }
};

// TODO: do we need this?
lightsource_t lighsource_no = {
    LIGHTSOURCE_NO,
    0, 0, 0, { }
};

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
}
