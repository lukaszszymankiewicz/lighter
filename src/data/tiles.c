#include <stdbool.h>

#include "library.h"

tile_blueprint_t *tiles_library[TILE_ALL] = {NULL};

tile_blueprint_t tile_blueprint_nothing = {
    TILESET_BASIC, 
    0.0, 0.0, 32.0/512.0, 1.0,
    false
};

tile_blueprint_t tile_blueprint_wall = {
    TILESET_BASIC, 
    32.0/512.0, 0.0, 64.0/512.0, 1.0,
    false
};

tile_blueprint_t tile_blueprint_broken_wall = {
    TILESET_BASIC,
    64.0/512.0, 0.0, 96.0/512.0, 1.0,
    false
};

tile_blueprint_t tile_blueprint_black_obstacle = {
    TILESET_BASIC,
    96.0/512.0, 0.0, 128.0/512.0, 1.0,
    true
};

void LIB_create_tile_library(
) {
    tiles_library[TILE_NOTHING]     = &tile_blueprint_nothing ;
    tiles_library[TILE_WALL]        = &tile_blueprint_wall;
    tiles_library[TILE_BROKEN_WALL] = &tile_blueprint_broken_wall;
    tiles_library[TILE_BLACK]       = &tile_blueprint_black_obstacle;
};
