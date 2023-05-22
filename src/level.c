#include "global.h"
#include "gfx.h"
#include "tile.h"
#include "level.h"
#include "primitives.h"
#include "segment.h"
#include <math.h>
#include <stdio.h>

enum { W, S, A, D };

// this stores temporary tile data to determine if tile is an obstacle
typedef struct cell {
    int edge[4];
} cell_t;

const static int EMPTY_CELL = -1;

level_t* LVL_new(
) {
    level_t *new_level           = NULL;
    new_level                    = (level_t*)malloc(sizeof(level_t));

    new_level->n_fill            = 0;
    new_level->structure         = NULL;
    new_level->obstacles         = NULL;
    new_level->obstacle_segments = NULL;

    new_level->tileset           = NULL;
    new_level->tileset           = (texture_t*)malloc(sizeof(texture_t));

    return new_level;
}

void LVL_add_entity_fill(
    level_t *level,
    int x, int y, int id
) {
    level->entities_fill[level->n_fill++] = (entity_fill_t) {x, y, id};
}

// initialise level structure
void LVL_clean_structure(
    level_t *level
) {
    for (int x=0; x<level->size_x; x++) { 
        for (int y=0; y<level->size_y; y++) { 
            level->structure[y * level->size_x + x] = NULL;
        }
    }
}

void LVL_clean_obstacles(
    level_t *level
) {
    for (int x=0; x<level->size_x; x++) { 
        for (int y=0; y<level->size_y; y++) { 
            level->obstacles[y * level->size_x + x] = 0;
        }
    }
}

// allocs proper memory to fit all tiles of level
void LVL_set_size(
    level_t *level,
    int      size_x,
    int      size_y
) {
    level->size_x    = size_x;
    level->size_y    = size_y;

    level->structure = malloc(sizeof (tile_t) * level->size_x * level->size_y);
    LVL_clean_structure(level);

    level->obstacles = malloc(sizeof (int)    * level->size_x * level->size_y);
    LVL_clean_obstacles(level);
}

// fills level structure with one tile
void LVL_fill_structure(
    level_t *level,
    int      x,
    int      y,
    int      i
) {
    int tileset_x;
    int tileset_y;
    tile_t *tile    = NULL;

    // determine place in the tileset
    int tile_per_row = (int)((float)TXTR_width(level->tileset) / (float)TILE_WIDTH);
    tileset_x = (i % tile_per_row) * TILE_WIDTH;
    tileset_y = (i / tile_per_row) * TILE_HEIGHT;

    tile = TILE_new(
        level->tileset,
        tileset_x,
        tileset_y,
        tileset_x + TILE_WIDTH,
        tileset_y + TILE_HEIGHT,
        x * TILE_WIDTH,
        y * TILE_HEIGHT
    );
    level->structure[y * level->size_x + x] = tile;
}

// fills level obstacles (1-obstacle, 0-no obstacle) with single value
void LVL_fill_obstacle(
    level_t *level,
    int      x,
    int      y,
    int      i
) {
    level->obstacles[y * level->size_x + x] = i;
}

tile_t* LVL_tile_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    if (x>0 && x<level->size_x && y>0 && y<level->size_y) {
        return level->structure[y * level->size_x + x];
    }
    return NULL;
}

int LVL_obstacle_on_pos(level_t* level, int x, int y) {
    if (x<0 || x>level->size_x || y<0 || y>level->size_y) {
        return 0;
    }
    return level->obstacles[y * level->size_x + x];
}

texture_t* LVL_get_tile_texture(
    level_t *level
) {
    return level->tileset;
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

    segment_t *ptr           = NULL;
    cell_t cells[level->size_x][level->size_y];

    for (int yy = 0; yy < level->size_x; yy++) {
        for (int xx = 0; xx < level->size_y; xx++) {
            cells[yy][xx].edge[W] = EMPTY_CELL;
            cells[yy][xx].edge[S] = EMPTY_CELL;
            cells[yy][xx].edge[A] = EMPTY_CELL;
            cells[yy][xx].edge[D] = EMPTY_CELL;
        }
    }

    int index        = EMPTY_CELL;
    int obstacle_num = 0;

    for (int x=0; x<level->size_x; x++) {
        for (int y=0; y<level->size_y; y++) {

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

void LVL_draw(
    level_t *level,
    int      camera_x,
    int      camera_y
) {
    int st_x           = camera_x - ENTITY_DRAW_X_RANGE;
    int st_y           = camera_y - ENTITY_DRAW_Y_RANGE;

    int st_tile_pos_x  = st_x / TILE_WIDTH;
    int st_tile_pos_y  = st_y / TILE_HEIGHT;
    int end_tile_pos_x = st_tile_pos_x + SCREEN_TILE_PER_Y;
    int end_tile_pos_y = st_tile_pos_y + SCREEN_TILE_PER_X;

    // camera to absolute orientation
    float x_diff = (((float)camera_x) / (float)SCREEN_WIDTH) * global_x_scale;
    float y_diff = (((float)camera_y) / (float)SCREEN_HEIGHT) * global_y_scale;

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            tile_t    *tile    = NULL;
            texture_t *texture = NULL;

            tile    = LVL_tile_on_pos(level, x, y);
            texture = LVL_get_tile_texture(level);
            
            if (tile != NULL) {

                printf(
                    "am I correct? %f %f %f %f\n",
                    tile->coord.x1 - x_diff,
                    tile->coord.y1 + y_diff,
                    tile->coord.x2 - x_diff,
                    tile->coord.y2 + y_diff
                ); 

                GFX_render_texture_part(
                    texture,
                    tile->coord.x1 - x_diff,
                    tile->coord.y1 + y_diff,
                    tile->coord.x2 - x_diff,
                    tile->coord.y2 + y_diff,
                    tile->img.x1,
                    tile->img.y1,
                    tile->img.x2,
                    tile->img.y2,
                    false
                );
            }
        }

    }
}

void LVL_free(
    level_t *level
) {
    TXTR_free(level->tileset);
    level->tileset = NULL;

    free(level->structure);
    level->structure = NULL;

    free(level->obstacles);
    level->obstacles = NULL;

    SEG_free(level->obstacle_segments);
    level->obstacle_segments = NULL;

    free(level);
    level = NULL;
}

