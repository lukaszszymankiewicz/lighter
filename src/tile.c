#include "global.h"
#include "tile.h"

tile_t* TILE_new(
    int x,
    int y
) {

    tile_t *new_tile = NULL;
    new_tile         = (tile_t*)malloc(sizeof(tile_t));

    new_tile->x      = x;
    new_tile->y      = y;
    new_tile->width  = TILE_WIDTH;
    new_tile->height = TILE_HEIGHT;

    return new_tile;
}
