#include "assets.h"
#include "global.h"
#include "primitives.h"
#include "tile.h"

#ifndef LEVEL_H
#define LEVEL_H


// struct for holding level data transalted to ready-to-use object (not raw data like mapfile_t)
typedef struct level {
    int            size_x;               // level width in tiles
    int            size_y;               // level height in tiles
    segment_t     *obstacle_segments;    // vector of segments, which are obstacles
    texture_t     *tileset;              // tileset
    tile_t        *tile_array;           // tiles from tilesets used, vector of unique tiles
    int           *obstacles;            // matrix of obstacles (1-obstacle 0-no obstacle)
                                         // it is one-to-one representation of level->structure, 
                                         // but only indicates if array is an obstacle
    tile_t        **structure;           // matrix of tiles (from tile_array), representing level
} level_t;

level_t* LVL_new();

void LVL_analyze(level_t *level);
void LVL_draw(level_t *level, int camera_x, int camera_y);

void LVL_fill_shadowbuffer_with_tiles(level_t *level, int x, int y, int obstacle);

tile_t* LVL_tile_on_pos(level_t *level, int x, int y);

void LVL_set_size(level_t *level, int size_x, int size_y);
void LVL_set_tile_number(level_t *level, int tile_number);
void LVL_set_tileset(level_t *level, texture_t* texture);
void LVL_add_tile(level_t *level, int tile_index, int x_offset, int y_offset);
void LVL_fill_structure(level_t *level, int x, int y, int i);
void LVL_fill_obstacle(level_t *level, int x, int y, int i);
void LVL_fill_tiles(level_t *level);

void LVL_draw_light(level_t *level);

level_t* LVL_read_level(const char *filename);
void LVL_free(level_t* level);

#endif
