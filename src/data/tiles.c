#include <stdbool.h>

#include "library.h"

tile_blueprint_t *tiles_library[TILE_ALL] = {NULL};

    
tile_blueprint_t tile_blueprint_nothing = {
    TILE_NOTHING, TILESET_BASIC, 0, 0, OBSTACLE_FALSE
};

tile_blueprint_t tile_blueprint_wall = {
    TILE_WALL, TILESET_BASIC, 1, 0, OBSTACLE_FALSE
};

tile_blueprint_t tile_blueprint_broken_wall = {
    TILE_BROKEN_WALL, TILESET_BASIC, 2, 0, OBSTACLE_FALSE 
};

tile_blueprint_t tile_blueprint_black_obstacle = {
    TILE_BLACK, TILESET_BASIC, 0, 0, OBSTACLE_TRUE
};

void LIB_read_tiles_blueprint(
    tile_blueprint_t* blueprint
) {
    if (blueprint) {
        printf("TILE BLUEPRINT ID = %d read\n", blueprint->id);
    }
    tiles_library[blueprint->id]      = blueprint;
};

void LIB_create_tile_library(
) {
    LIB_read_tiles_blueprint(&tile_blueprint_nothing);
    LIB_read_tiles_blueprint(&tile_blueprint_wall);
    LIB_read_tiles_blueprint(&tile_blueprint_broken_wall);
    LIB_read_tiles_blueprint(&tile_blueprint_black_obstacle);
};
