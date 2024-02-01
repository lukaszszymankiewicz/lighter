#include <stdbool.h>

#include "data/library.h"

#include "global.h"
#include "level.h"
#include "mat.h"
#include "scene.h"
#include "segment.h"
#include "tile.h"

const static int EMPTY_CELL = -1;

enum { W, S, A, D };

// this stores temporary tile data to determine if tile is an obstacle
typedef struct cell {
    int edge[4];
} cell_t;

int LVL_size_x(
    level_t *level
) {
    return levels_library[level->blueprint_id]->size_x;
}

int LVL_size_y(
    level_t *level
) {
    return levels_library[level->blueprint_id]->size_y;
}

int LVL_pos(
    level_t *level,
    int x,
    int y
) {
    // return y * LVL_size_x(level) + x;
    return (LVL_size_y(level) - y - 1) * LVL_size_x(level) + x;
}

void LVL_set_tile(
    level_t *level,
    tile_t  *tile,
    int x, int y
) {
    level->structure[LVL_pos(level, x, y)] = tile;
}

void LVL_clean_structure(
    level_t *level
) {
    for (int x=0; x<LVL_size_x(level); x++) { 
        for (int y=0; y<LVL_size_y(level); y++) { 
            LVL_set_tile(level, NULL, x, y);
        }
    }
}

int LVL_tile_blueprint_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    return levels_library[level->blueprint_id]->tiles[LVL_pos(level, x, y)];
}

int LVL_n_entity_fills(
    level_t *level
) {
    return levels_library[level->blueprint_id]->n_fills;
}

int LVL_tileset_id(
    level_t *level
) {
    int id = levels_library[level->blueprint_id]->tileset_id; 
    return tilesets_library[id]->id;
}

void LVL_fill(
    level_t* level
) {
    LVL_clean_structure(level);

    for (int x=0; x<LVL_size_x(level); x++) {
        for (int y=0; y<LVL_size_y(level); y++) {

            int blueprint_id = LVL_tile_blueprint_on_pos(level, x, y);
            tile_t *tile     = NULL;
            tile             = TILE_new(x * TILE_WIDTH, y * TILE_HEIGHT, blueprint_id);

            LVL_set_tile(level, tile, x, y);
        }
    }
}

level_t* LVL_new(
    int blueprint_id
) {
    level_t *new_level           = NULL;
    new_level                    = (level_t*)malloc(sizeof(level_t));

    new_level->blueprint_id      = blueprint_id;
    new_level->obstacle_segments = NULL;

    int tileset_id               = levels_library[blueprint_id]->tileset_id;
    new_level->tileset_id        = tilesets_library[tileset_id]->id;

    return new_level;
}

tile_t* LVL_tile_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    if (x>0 && x<LVL_size_x(level) && y>0 && y<LVL_size_y(level)) {
        return level->structure[LVL_pos(level, x, y)];
    }
    return NULL;
}

bool LVL_obstacle_on_pos2(
    level_t* level,
    int x,
    int y
) {
    if (x<0 || x>LVL_size_x(level) || y<0 || y>LVL_size_y(level)) {
        return false;
    }

    // int pos = y * LVL_size_x(level) + x;
    //return level->structure[pos]->obstacle;
    return level->structure[LVL_pos(level, x, y)]->obstacle;
}

bool LVL_obstacle_on_pos(
    level_t* level,
    int x,
    int y
) {
    if (x<0 || x>LVL_size_x(level) || y<0 || y>LVL_size_y(level)) {
        return false;
    }
    return level->structure[LVL_pos(level, x, y)]->obstacle;
}

// fills level obstacle_segments.
void LVL_analyze(
    level_t *level
) {
    // second analyze should erase old effect
    if (level->obstacle_segments) {
        free(level->obstacle_segments);
        level->obstacle_segments = NULL;
    }

    int size_x = LVL_size_x(level);
    int size_y = LVL_size_y(level);

    segment_t *ptr           = NULL;
    cell_t cells[size_x][size_y];

    for (int yy=0; yy<size_x; yy++) {
        for (int xx=0; xx<size_y; xx++) {
            cells[yy][xx].edge[W] = EMPTY_CELL;
            cells[yy][xx].edge[S] = EMPTY_CELL;
            cells[yy][xx].edge[A] = EMPTY_CELL;
            cells[yy][xx].edge[D] = EMPTY_CELL;
        }
    }

    int index        = EMPTY_CELL;
    int obstacle_num = 0;

    for (int x=0; x<size_x; x++) {
        for (int y=0; y<size_y; y++) {

            int tile_pos_x = TILE_WIDTH  * x;
            int tile_pos_y = TILE_HEIGHT * y;

            // ANALYZING OBSTACLES
            if (LVL_obstacle_on_pos(level, x, y)) {

                // check LEFT border
                if (LVL_obstacle_on_pos(level, x-1, y) == 0) {
                    // create new
                    if (cells[y-1][x].edge[A] == EMPTY_CELL) {
                        SEG_push(
                          &level->obstacle_segments,
                          tile_pos_x,
                          tile_pos_y,
                          tile_pos_x,
                          tile_pos_y + TILE_HEIGHT
                        );

                        cells[y][x].edge[A] = obstacle_num;
                        obstacle_num++;
                    }

                    // borrow from up
                    else {
                        index = cells[y-1][x].edge[A];
                        cells[y][x].edge[A] = index;
                        ptr = SEG_get(level->obstacle_segments, index);
                        ptr->y2 += TILE_HEIGHT;
                    }
                }

                // check RIGHT border
                if (LVL_obstacle_on_pos(level, x+1, y) == 0) {
                    // create new
                    if (cells[y-1][x].edge[D] == EMPTY_CELL) {
                        SEG_push(
                          &level->obstacle_segments,
                          tile_pos_x + TILE_WIDTH,
                          tile_pos_y,
                          tile_pos_x + TILE_WIDTH,
                          tile_pos_y + TILE_HEIGHT
                        );

                        cells[y][x].edge[D] = obstacle_num;
                        obstacle_num++;
                    }

                    // borrow from up
                    else {
                        index = cells[y-1][x].edge[D];
                        cells[y][x].edge[D] = index;
                        ptr = SEG_get(level->obstacle_segments, index);
                        ptr->y2 += TILE_HEIGHT;
                    }
                }

                // check UP line
                if (!LVL_obstacle_on_pos(level, x, y-1)) {
                    // create new
                    if (cells[y][x-1].edge[W] == EMPTY_CELL) {
                        SEG_push(
                          &level->obstacle_segments,
                          tile_pos_x,
                          tile_pos_y,
                          tile_pos_x + TILE_WIDTH,
                          tile_pos_y
                        );

                        cells[y][x].edge[W] = obstacle_num;
                        obstacle_num++;
                    }

                    // borrow from left
                    else {
                        index = cells[y][x-1].edge[W];
                        cells[y][x].edge[W] = index;
                        ptr = SEG_get(level->obstacle_segments, index);
                        ptr->x2 += TILE_WIDTH;
                    }
                }

                // check BOTTOM line
                if (!LVL_obstacle_on_pos(level, x, y+1)) {
                    // create new
                    if (cells[y][x-1].edge[S] == EMPTY_CELL) {
                        SEG_push(
                          &level->obstacle_segments,
                          tile_pos_x,
                          tile_pos_y + TILE_HEIGHT,
                          tile_pos_x + TILE_WIDTH,
                          tile_pos_y + TILE_HEIGHT
                        );

                        cells[y][x].edge[S] = obstacle_num;
                        obstacle_num++;
                    }

                    // borrow from left
                    else {
                        index = cells[y][x-1].edge[S];
                        cells[y][x].edge[S] = index;
                        ptr = SEG_get(level->obstacle_segments, index);
                        ptr->x2 += TILE_WIDTH;
                    }
                }
            }
        }
    }
}

void LVL_put_to_scene(
    level_t *level
) {
    int id             = levels_library[level->blueprint_id]->tileset_id; 
    int tileset        = LVL_tileset_id(level);
    int texture_w      = tilesets_library[id]->surface->w;
    int texture_h      = tilesets_library[id]->surface->h;
    
    // start drawing from st_x and st_y pixels
    int st_x           = (camera_x + ENTITY_DRAW_X_RANGE) / TILE_WIDTH;
    int st_y           = (camera_y + ENTITY_DRAW_Y_RANGE) / TILE_HEIGHT;

    // start reading tiles from st_tile_pos_x and st_tile_pos_y 
    // end at end_tile_pos_x and end_tile_pos_y
    int st_tile_pos_x  = st_x - (SCREEN_WIDTH  / TILE_WIDTH  / 2) - 2;
    int st_tile_pos_y  = st_y - (SCREEN_HEIGHT / TILE_HEIGHT / 2) - 2;
    int end_tile_pos_x = st_x + (SCREEN_WIDTH  / TILE_WIDTH  / 2) + 2;
    int end_tile_pos_y = st_y + (SCREEN_HEIGHT / TILE_HEIGHT / 2) + 2;
    
    array_t *vertices = NULL;

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            tile_t *tile = NULL;
            tile         = LVL_tile_on_pos(level, x, y);
            
            if (!tile) {
                continue; 
            }

            // gather tiles vertices
            array_t *new_vertices = SCENE_texture_pos(
                tile->x,                tile->y,
                TILE_WIDTH,             TILE_HEIGHT,
                tile->row * TILE_WIDTH, tile->col * TILE_HEIGHT,
                TILE_WIDTH,             TILE_HEIGHT,
                texture_w,              texture_h,
                false,                  false
            );

            vertices = MAT_append(vertices, new_vertices);
        }
    }

    // put all gathered vertices on scene
    SCENE_put_texture_to_scene("tiles", vertices, tileset);
}

void LVL_free(
    level_t *level
) {
    if (levels_library[level->blueprint_id]) {
        for (int x=0; x<LVL_size_x(level); x++) {
            for (int y=0; y<LVL_size_y(level); y++) {
                TILE_free(level->structure[LVL_pos(level, x, y)]);
            }
        }
    }

    SEG_free(level->obstacle_segments);
    level->obstacle_segments = NULL;
    
    if (level) {
        free(level);
    }
    level = NULL;
}

