#include "global.h"
#include "primitives.h"
#include "tile.h"

#ifndef LEVEL_H
#define LEVEL_H

// struct for holding level data transalted to ready-to-use object (not raw data like mapfile_t)
typedef struct level {
    int          hero_x;
    int          hero_y;
    int          size_x;
    int          size_y;
    int          tiles_used;           // length of tileset
    int         *obstacles;            // matrix of obstacles (1-obstacle 0-no obstacle)
    segment_t   *obstacle_segments;    // matrix of obstacles converted into segments
    tile_t      *tile_array;           // tiles from tilesets used
    texture_t   *tileset;              // tileset
    tile_t      **structure;           // structure of tiles creating level
} level_t;

level_t* LVL_new();

void LVL_draw(level_t* level, int hero_x, int hero_y);
void LVL_analyze(level_t *level, int hero_x, int hero_y);
void LVL_fill_shadowbuffer_with_tiles(level_t *level, int hero_x, int hero_y, int obstacle);

void LVL_set_size(level_t *level, int size_x, int size_y);
void LVL_set_tile_number(level_t *level, int tile_number);
void LVL_set_tileset(level_t *level, texture_t* texture);

tile_t* LVL_tile_on_pos(level_t *level, int x, int y);

void LVL_add_tile(level_t *level, int tile_index, int x_offset, int y_offset);
void LVL_fill_structure(level_t *level, int x, int y, int i);
void LVL_fill_obstacle(level_t *level, int x, int y, int i);
void LVL_fill_tiles(level_t *level);

void LVL_free(level_t* level);

#endif
