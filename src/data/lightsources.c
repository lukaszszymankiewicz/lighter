#include <stdio.h>

#include "../source.h"
#include "../global.h"

#include "library.h"

lightsource_t* lighsources_library[LIGHTSOURCE_ALL] = { NULL };

lightsource_t lighsource_wallight = {
    PI/6, 7, 2, 
    {
        (lightpolygon_t){0, 0, 226, 13, 13, 50, 0, 0},
        (lightpolygon_t){0, 0, 226, 13, 13, 50, 0, PI},
    }
};

lightsource_t lighsource_lighter = {
    PI/7, 7, 1, 
    {
        (lightpolygon_t){0, -2, 200, 200, 120, 30, 20, 0},
        // (lightpolygon_t){0,  0, DEFAULT_LIGHT_R, DEFAULT_LIGHT_G, DEFAULT_LIGHT_B, 50, 0, 0},
        // (lightpolygon_t){0, +2, 200, 200, 120, 20, 20, 0},
    }
};

lightsource_t lighsource_no = {
    0, 0, 0, { }
};

void LIB_create_lightsources_library(
) {
    lighsources_library[LIGHTSOURCE_LIGHTER]   = &lighsource_lighter;
    lighsources_library[LIGHTSOURCE_WALLLIGHT] = &lighsource_wallight;
    lighsources_library[LIGHTSOURCE_NO]        = &lighsource_no;
}
