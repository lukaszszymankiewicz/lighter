#include <stdlib.h>

#include "render.h"
#include "global.h"
#include "tile.h"

tile_t* TILE_new(
    int        blueprint_id,
    int        tileset_id,
    float      clip_x1,      // clip of the tile from texture (in pixels)
    float      clip_y1,      // clip of the tile from texture (in pixels)
    float      clip_x2,      // clip of the tile from texture (in pixels)
    float      clip_y2,      // clip of the tile from texture (in pixels)
    int        x,            // x of tile on level (left, down coord) in px
    int        y             // y of tile on level (left, down coord) in px
) {
    tile_t *new_tile       = NULL;
    new_tile               = (tile_t*)malloc(sizeof(tile_t));

    float render_x1, render_y1, render_x2, render_y2;
        
    // TODO: this should be moved to separate function
    render_x1    = (float)x / (float)SCREEN_WIDTH;
    render_y1    = (float)y / (float)SCREEN_HEIGHT;

    render_y1   *= -1;

    render_x2    = render_x1 + WIDTH_COEF;
    render_y2    = render_y1 - HEIGHT_COEF;
    
    render_x1   *= global_x_scale;
    render_y1   *= global_y_scale;
    render_x2   *= global_x_scale;
    render_y2   *= global_y_scale;

    new_tile->blueprint_id = blueprint_id;
    new_tile->tileset_id   = tileset_id;
    new_tile->coord        = (render_coord_t) {render_x1, render_y1, render_x2, render_y2};
    new_tile->img          = (render_coord_t) {clip_x1, clip_y1, clip_x2, clip_y2};

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
