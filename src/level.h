#include "global.h"
#include "primitives.h"
#include "tile.h"
#include "texture.h"

#ifndef LEVEL_H
#define LEVEL_H

#define MAX_ENTITY_FILL 10

// this struct holds entity to be placed on level, obviously there is separate part of the code
// where entities are managed, so this middlewere is used
typedef struct entity_fill {
    int x; int y; int id;
} entity_fill_t;

typedef struct level {
    int            size_x;                         // level width in tiles
    int            size_y;                         // level height in tiles
    int            n_fill;                         // number of entities to fill 
    entity_fill_t  entities_fill[MAX_ENTITY_FILL]; // entities to fill
    segment_t     *obstacle_segments;              // vector of segments, which are obstacles
    texture_t     *tileset;                        // tileset texture
    int           *obstacles;                      // matrix of obstacles (1-obstacle 0-no obstacle)
                                                   // it is one-to-one representation of level->structure, 
                                                   // but only indicates if array is an obstacle
    tile_t        **structure;                     // matrix of tiles, representing level
} level_t;

level_t* LVL_new();

void LVL_analyze(level_t *level);
void LVL_draw(level_t *level, int camera_x, int camera_y);
void LVL_set_size(level_t *level, int size_x, int size_y);
void LVL_fill_structure(level_t *level, int x, int y, int i);
void LVL_fill_obstacle(level_t *level, int x, int y, int i);
void LVL_add_entity_fill(level_t *level, int x, int y, int id);

void LVL_free(level_t* level);

tile_t* LVL_tile_on_pos(level_t *level, int x, int y);

#endif
