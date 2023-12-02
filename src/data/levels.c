#include "library.h"

#include "../level.h"

level_blueprint_t* levels_library[LEVEL_ALL] = {NULL};

level_blueprint_t level_blueprint_sample = {
    0,
    TILESET_BASIC,
    11, 11,
    {
        0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,
        0,0,3,3,3,3,3,3,3,0,0,
        0,0,3,1,1,1,1,1,3,0,0,
        0,0,3,1,0,0,0,0,3,0,0,
        0,0,3,3,0,0,0,3,3,0,0,
        0,0,3,1,1,1,1,1,3,0,0,
        0,0,3,3,3,3,3,3,3,0,0,
        0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0
    },    
    2,
    {
        (entity_fill_t) {5, 4, 0},
        (entity_fill_t) {4, 6, 2}
    }
};

level_blueprint_t level_blueprint_testing = {
    0,
    TILESET_BASIC,
    10, 10,
    {
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0
    },    
    0, { }
};

void LIB_create_levels_library(
) {
    levels_library[LEVEL_SAMPLE] = &level_blueprint_sample;
    levels_library[LEVEL_TEST]   = &level_blueprint_testing;
};

