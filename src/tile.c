#include <stdlib.h>

#include "data/library.h"

#include "gl_util.h"
#include "global.h"
#include "tile.h"

render_coord_t TILE_clip(
    int tileset_id,
    int row,
    int col
) { 
    int w = tilesets_library[tileset_id]->surface->w;
    int h = tilesets_library[tileset_id]->surface->h;

    return GL_UTIL_clip(
         row    * TILE_WIDTH,  col    * TILE_HEIGHT,
        (row+1) * TILE_WIDTH, (col+1) * TILE_HEIGHT,
         w,                    h
    );
}

tile_t* TILE_new(
    int            blueprint_id,
    int            tileset_id,
    int            row,
    int            col,
    int            x,            // x of tile on level (left, down coord) in px
    int            y             // y of tile on level (left, down coord) in px
) {
    tile_t *new_tile       = NULL;
    new_tile               = (tile_t*)malloc(sizeof(tile_t));
    
    render_coord_t render  = GL_UTIL_rect(x, y, TILE_WIDTH, TILE_HEIGHT);
    new_tile->blueprint_id = blueprint_id;
    new_tile->tileset_id   = tileset_id;
    new_tile->render       = render;
    new_tile->clip         = TILE_clip(tileset_id, row, col);

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
