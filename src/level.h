#include "global.h"
#include "primitives.h"
#include "tile.h"

#ifndef LEVEL_H
#define LEVEL_H

// struct for holding level data transalted to ready-to-use object (not raw data like mapfile_t)
typedef struct level {
    int          size_x;
    int          size_y;
    int          tiles_used;           // length of tileset
    int         *obstacles;            // matrix of obstacles 1-obstacle 0-no obstacle
    segment_t   *obstacle_segments;    // matrix of obstacles converted into segments
    texture_t   *tileset_array;        // tilesets array holding all tiles
    tile_t      *tile_array;           // tiles from tilesets used
    tile_t      **structure;           // structure of tiles creating level
} level_t;

// struct for holding level data read from file
typedef struct mapfile {
    char     line_buffer[MAX_LINE_LENGHT];
    FILE    *file_pointer;
} mapfile_t;

level_t* LVL_new();
void LVL_draw(level_t* level, int hero_x, int hero_y);
level_t* LVL_read_from_file(char* filename);
void LVL_free(level_t* level);
void LVL_analyze(level_t *level, int hero_x, int hero_y);
void LVL_draw(level_t *level, int hero_x, int hero_y);
void LVL_fill_shadowbuffer_with_tiles(level_t *level, int hero_x, int hero_y, int obstacle);

#endif
