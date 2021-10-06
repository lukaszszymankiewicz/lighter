#include "def.h"
#include "tile.h"
#include "primitives.h"
#include "obstacle.h"


tile_t* TILE_new(
    int tileset_index,
    int x_offset,
    int y_offset
) {

    tile_t *new_tile = NULL;

    new_tile = (tile_t*)malloc(sizeof(tile_t));

    new_tile->index  = tileset_index;
    new_tile->x      = x_offset;
    new_tile->y      = y_offset;
    new_tile->width  = TILE_WIDTH;
    new_tile->height = TILE_HEIGHT;

    return new_tile;
}


// Converts tiles of level to list of segments on which ray light can hit.
// obstacle_t* TILE_calculate_ray_obstacles(tiles_list_t* tiles) {
//     obstacle_t* obstacles = NULL;
//     tiles_list_t* ptr = NULL;
// 
//     ptr = tiles;
// 
//     while (ptr) {
//         OBS_push(&obstacles, ptr->tile->x1, ptr->tile->y1, ptr->tile->x1, ptr->tile->y2);
//         OBS_push(&obstacles, ptr->tile->x1, ptr->tile->y2, ptr->tile->x2, ptr->tile->y2);
//         OBS_push(&obstacles, ptr->tile->x2, ptr->tile->y2, ptr->tile->x2, ptr->tile->y1);
//         OBS_push(&obstacles, ptr->tile->x2, ptr->tile->y1, ptr->tile->x1, ptr->tile->y1);
// 
//         ptr=ptr->next;
//     }
//     return obstacles;
// }
// 
