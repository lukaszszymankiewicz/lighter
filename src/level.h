#include "tile.h"
#include "primitives.h"
#include "texture.h"
#include "config.h"

#ifndef LEVEL_H
#define LEVEL_H

typedef struct level {
    int          size_x;
    int          size_y;
    int         *obstacles;            // matrix of obstacles
    segment_t   *obstacle_segments;    // matrix of obstacles converted into segments
    texture_t   *tileset_array;        // tilesets used
    tile_t      *tile_array;           // tiles from tilesets used
    tile_t      **structure;           // matrix of tiles
} level_t;

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

#endif
