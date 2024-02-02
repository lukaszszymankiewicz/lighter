#include "library.h"

#include "../level.h"

level_blueprint_t* levels_library[LEVEL_ALL] = {NULL};

level_blueprint_t level_blueprint_sample = {
    LEVEL_SAMPLE,
    TILESET_BASIC,
    11, 20,
    {
        0,0,0,0,0,0,0,0,0,0,0,
        2,3,3,3,3,3,3,3,3,3,2,
        2,3,2,1,1,1,2,1,2,3,2,
        2,3,1,1,1,0,2,1,2,3,2,
        2,3,2,2,0,1,1,1,2,3,2,
        2,3,1,1,1,3,2,3,2,3,2,
        2,3,2,1,2,1,2,0,2,3,2,
        2,3,2,2,3,3,3,3,3,3,2,
        2,3,2,2,3,2,2,2,2,3,2,
        2,3,3,3,3,2,2,2,2,3,2,
        0,3,0,0,0,0,0,0,0,3,0,
        0,3,0,0,0,0,0,0,0,3,0,
        0,3,0,0,0,0,0,0,0,3,0,
        0,3,0,0,0,0,0,0,0,3,0,
        0,3,0,0,0,0,0,0,0,3,0,
        0,3,0,0,0,0,0,0,0,3,0,
        0,3,0,0,0,0,0,0,0,3,0,
        0,3,0,0,0,0,0,0,0,3,0,
        0,3,3,3,3,3,3,3,3,3,0,
        0,0,0,0,0,0,0,0,0,0,0,
        // max = 219
    },    
    2,
    {
        (entity_fill_t) {7, 13, ENTITY_HERO},
        (entity_fill_t) {4, 15, ENTITY_LAMP_SMALL},
        // (entity_fill_t) {6, 6, ENTITY_LAMP_SMALL}
    }
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
};

