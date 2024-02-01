#include "library.h"

#include "../level.h"

level_blueprint_t* levels_library[LEVEL_ALL] = {NULL};

level_blueprint_t level_blueprint_sample = {
    LEVEL_SAMPLE,
    TILESET_BASIC,
    11, 11,
    {
        0,0,0,0,0,0,0,0,0,0,0,
        0,3,3,3,3,3,3,3,3,3,0,
        0,3,2,1,1,1,2,1,0,3,0,
        0,3,1,1,1,1,0,1,0,3,0,
        0,3,0,0,1,1,1,1,0,3,0,
        0,3,1,3,1,3,2,3,0,3,0,
        0,3,0,1,2,1,1,0,0,3,0,
        0,3,0,0,3,3,3,3,3,3,0,
        0,3,0,0,3,0,0,0,0,0,0,
        0,3,3,3,3,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0
    },    
    2,
    {
        (entity_fill_t) {7, 4, ENTITY_HERO},
        (entity_fill_t) {4, 7, ENTITY_LAMP_SMALL},
        // (entity_fill_t) {6, 6, ENTITY_LAMP_SMALL}
    }
};

level_blueprint_t level_blueprint_testing = {
    LEVEL_TEST,
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

void LIB_read_level_blueprint(
    level_blueprint_t* blueprint
) {
    if (blueprint) {
        printf("LEVEL BLUEPRINT ID = %d read\n", blueprint->id);
    }
    levels_library[blueprint->id]      = blueprint;
};

void LIB_create_levels_library(
) {
    LIB_read_level_blueprint(&level_blueprint_sample);
    LIB_read_level_blueprint(&level_blueprint_testing);
};

