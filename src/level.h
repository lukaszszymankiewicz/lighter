#include "assets.h"
#include "global.h"
#include "entity.h"
#include "primitives.h"
#include "tile.h"

#ifndef LEVEL_H
#define LEVEL_H

#define MAX_ENTITY 32
#define ENTITY_UPDATE_LIGHT_X_RANGE     SCREEN_WIDTH * 2
#define ENTITY_UPDATE_LIGHT_Y_RANGE     SCREEN_HEIGHT * 2
#define ENTITY_UPDATE_X_RANGE           SCREEN_WIDTH
#define ENTITY_UPDATE_Y_RANGE           SCREEN_HEIGHT
#define ENTITY_DRAW_X_RANGE             SCREEN_WIDTH / 2
#define ENTITY_DRAW_Y_RANGE             SCREEN_HEIGHT / 2
#define SCREEN_TILE_PER_X               10
#define SCREEN_TILE_PER_Y               12
#define COLLISION_UPDATE_X_RANGE        100
#define COLLISION_UPDATE_Y_RANGE        100

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
    entity_t      *entities[MAX_ENTITY];
} level_t;

extern level_t           *levels[ASSET_LEVEL_ALL];

level_t* LVL_new();

void LVL_analyze(level_t *level);
void LVL_draw(level_t* level);
void LVL_fill_shadowbuffer_with_tiles(level_t *level, int x, int y, int obstacle);
int LVL_add_entity(level_t* level, entity_t *entity, int id);
entity_t* LVL_get_entity_by_id(level_t* level, int id);

tile_t* LVL_tile_on_pos(level_t *level, int x, int y);

void LVL_set_size(level_t *level, int size_x, int size_y);
void LVL_set_tile_number(level_t *level, int tile_number);
void LVL_set_tileset(level_t *level, texture_t* texture);
void LVL_add_tile(level_t *level, int tile_index, int x_offset, int y_offset);
void LVL_fill_structure(level_t *level, int x, int y, int i);
void LVL_fill_obstacle(level_t *level, int x, int y, int i);
void LVL_fill_tiles(level_t *level);

int LVL_hero_x(level_t* level);
int LVL_hero_y(level_t* level);

bool LVL_entity_in_range(level_t *level, entity_t *entity, int range_x, int range_y);
void LVL_apply_collision(level_t* level, entity_t *entity);
void LVL_update_entities(level_t *level);
void LVL_draw_entities(level_t *level);

level_t* LVL_read_level(const char *filename);
void LVL_free(level_t* level);

#endif
