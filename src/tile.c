#include <stdlib.h>

#include "data/library.h"

#include "tile.h"

tile_t* TILE_new(
    int            x,        
    int            y,         
    int            blueprint_id
) {
    tile_t *new_tile       = NULL;
    new_tile               = (tile_t*)malloc(sizeof(tile_t));
    
    new_tile->x            = x;
    new_tile->y            = y;
    
    new_tile->row          = tilesets_library[blueprint_id]->row;
    new_tile->col          = tilesets_library[blueprint_id]->col;
    new_tile->obstacle     = tilesets_library[blueprint_id]->obstacle;
    new_tile->tileset_id   = tilesets_library[blueprint_id]->tileset_id;

    return new_tile;
}

void TILE_free(
    tile_t *tile
) {
    if (tile) {
        free(tile);    
    }
    tile = NULL;
}
