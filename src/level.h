#include "global.h"
#include "segment.h"
#include "tile.h"

#ifndef LEVEL_H
#define LEVEL_H

#define MAX_LEVEL_X_SIZE      20
#define MAX_LEVEL_Y_SIZE      20
#define MAX_LEVEL_ENTITY_FILL 10
#define MAX_LEVEL_SIZE        MAX_LEVEL_X_SIZE * MAX_LEVEL_Y_SIZE

typedef struct level {
    int           blueprint_id;
    int           tileset_id;
    tile_t        *structure[MAX_LEVEL_SIZE]; // matrix of tiles
    segment_t     *obstacle_segments;         // obstacle segments
} level_t;

level_t* LVL_new();

void LVL_analyze(level_t *level);
void LVL_put_to_scene(level_t *level);
void LVL_fill(level_t* level);
void LVL_free(level_t* level);

int LVL_tileset_id(level_t *level);
int LVL_n_entity_fills(level_t *level);

bool LVL_obstacle_on_pos(level_t* level, int x, int y);

tile_t* LVL_tile_on_pos(level_t *level, int x, int y);

#endif
