#include "library.h"

#include "../level.h"

#define MAX_X_SIZE 20
#define MAX_Y_SIZE 20
#define MAX_SIZE MAX_X_SIZE * MAX_Y_SIZE
#define MAX_ENTITY_FILL 10

level_t* levels_library[LEVEL_ALL] = {NULL};

typedef struct entity_fill {
    int x; int y; int id;
} entity_fill_t;

typedef struct level_blueprint {
    int           size_x;
    int           size_y;
    int           tiles[MAX_SIZE];
    entity_fill_t entities[MAX_ENTITY_FILL];
} level_blueprint_t;

level_blueprint_t level_blueprint_sample = {
    11, 11,
    {
        0,0,0,0,0,0,0,0,0,0,0,
        0,0,2,2,2,2,2,2,0,0,0,
        0,2,2,0,0,0,0,2,0,0,0,
        0,2,0,0,0,1,0,2,2,2,0,
        0,2,0,0,2,0,0,0,0,2,0,
        0,2,2,1,1,0,0,0,0,2,0,
        0,0,2,2,0,0,1,1,2,2,0,
        0,0,0,2,0,0,1,0,2,0,0,
        0,0,0,2,2,0,0,0,2,0,0,
        0,0,0,0,2,2,2,2,2,0,0,
        0,0,0,0,0,0,0,0,0,0,0,
    },    
    {
       (entity_fill_t) { 0,0,0 },      
       (entity_fill_t) { 0,0,0 },      
       (entity_fill_t) { 0,0,0 },      
    }
};

void LIB_create_levels_library(
) {
    levels_library[LEVEL_SAMPLE] = LIB_read_level(level_blueprint_sample);
};

