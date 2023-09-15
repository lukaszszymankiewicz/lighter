#include <stdlib.h>

#include "gl_util.h"
#include "global.h"
#include "tile.h"

tile_t* TILE_new(
    int            blueprint_id,
    int            tileset_id,
    render_coord_t clip,
    int            x,            // x of tile on level (left, down coord) in px
    int            y             // y of tile on level (left, down coord) in px
) {
    tile_t *new_tile       = NULL;
    new_tile               = (tile_t*)malloc(sizeof(tile_t));
    
    render_coord_t render  = GL_UTIL_rect(x, y, TILE_WIDTH, TILE_HEIGHT);

    new_tile->blueprint_id = blueprint_id;
    new_tile->tileset_id   = tileset_id;
    new_tile->coord        = render;
    new_tile->img          = clip;

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
