#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "data/library.h"

#include "gl_util.h"
#include "global.h"
#include "level.h"
#include "scene.h"
#include "segment.h"
#include "texture.h"
#include "tile.h"

#define LEVEL_VERTICES_FOR_SCENE 2500

enum { W, S, A, D };

// this stores temporary tile data to determine if tile is an obstacle
typedef struct cell {
    int edge[4];
} cell_t;

const static int EMPTY_CELL = -1;

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

void LVL_set_tile(
    level_t *level,
    tile_t  *tile,
    int x, int y
) {
    int size_x = LVL_size_x(level);
    level->structure[y * size_x + x] = tile;
}

void LVL_clean_structure(
    level_t *level
) {
    int size_x = LVL_size_x(level);
    int size_y = LVL_size_y(level);

    for (int x=0; x<size_x; x++) { 
        for (int y=0; y<size_y; y++) { 
            LVL_set_tile(level, NULL, x, y);
        }
    }
}

int LVL_tile_blueprint_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    int size_x = LVL_size_x(level);

    return levels_library[level->blueprint_id]->tiles[y * size_x + x];
}

int LVL_tile_blueprint_tileset_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    int id = LVL_tile_blueprint_on_pos(level, x, y);
    return tiles_library[id]->tileset_id;
}

int LVL_n_entity_fills(
    level_t *level
) {
    return levels_library[level->blueprint_id]->n_fills;
}

int LVL_tileset_id(
    level_t *level
) {
    return levels_library[level->blueprint_id]->tileset_id;
}

void LVL_fill_structure(
    level_t *level,
    int      x,
    int      y,
    int      id
) {
    tile_t *tile    = NULL;

    tile = TILE_new(
        id,
        tiles_library[id]->tileset_id,
        tiles_library[id]->clip,
        x * TILE_WIDTH,
        y * TILE_HEIGHT
    );

    LVL_set_tile(level, tile, x, y);
}

void LVL_fill(
    level_t* level
) {
    LVL_clean_structure(level);

    int size_x = LVL_size_x(level);
    int size_y = LVL_size_y(level);
    
    for (int x=0; x<size_x; x++) {
        for (int y=0; y<size_y; y++) {
            int id = LVL_tile_blueprint_on_pos(level, x, y);
            LVL_fill_structure(level, x, y, id);
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

    return new_level;
}

tile_t* LVL_tile_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    int size_x = LVL_size_x(level);
    int size_y = LVL_size_y(level);

    if (x>0 && x<size_x && y>0 && y<size_y) {
        return level->structure[y * size_x + x];
    }
    return NULL;
}

bool LVL_obstacle_on_pos(
    level_t* level,
    int x,
    int y
) {
    int size_x = LVL_size_x(level);
    int size_y = LVL_size_y(level);

    if (x<0 || x>size_x || y<0 || y>size_y) {
        return false;
    }
    int tile_id = LVL_tile_blueprint_on_pos(level, x, y);

    return tiles_library[tile_id]->obstacle;
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

// gratest function of all time
int LVL_put_tile_on_scene(
    float  *v,
    tile_t *tile,
    int     i
) {
   // Position                                       Texcoords
   v[i++]=tile->coord.x1; v[i++]=tile->coord.y2; v[i++]=tile->img.x1; v[i++]=tile->img.y2; // Top-left
   v[i++]=tile->coord.x2; v[i++]=tile->coord.y2; v[i++]=tile->img.x2; v[i++]=tile->img.y2; // Top-right
   v[i++]=tile->coord.x2; v[i++]=tile->coord.y1; v[i++]=tile->img.x2; v[i++]=tile->img.y1; // Bottom-right
   v[i++]=tile->coord.x1; v[i++]=tile->coord.y2; v[i++]=tile->img.x1; v[i++]=tile->img.y2; // Top-left
   v[i++]=tile->coord.x2; v[i++]=tile->coord.y1; v[i++]=tile->img.x2; v[i++]=tile->img.y1; // Bottom-right
   v[i++]=tile->coord.x1; v[i++]=tile->coord.y1; v[i++]=tile->img.x1; v[i++]=tile->img.y1; // Bottom-left
                                                                                           //
   return i;
}

void LVL_put_on_scene(
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

    float camera_x_diff = (((float)camera_x) / (float)SCREEN_WIDTH) * global_x_scale;
    float camera_y_diff = (((float)camera_y) / (float)SCREEN_HEIGHT) * global_y_scale;
    float uniforms[MAX_SHADER_UNIFORMS_ARGS_LEN] = { camera_x_diff, camera_y_diff, 0.0, 0.0 };

    float *vertices = (float*)malloc(sizeof(float) *LEVEL_VERTICES_FOR_SCENE);
    int i = 0;

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            tile_t *tile = NULL;
            tile         = LVL_tile_on_pos(level, x, y);
            
            if (tile) {
                int z = LVL_put_tile_on_scene(vertices, tile, i); i = z;
            }

        }
    }

    SCENE_add(
        scene, 
        LAYER_TILE,
        LVL_tileset_id(level),
        i,
        vertices,
        uniforms
    );
}

void LVL_free(
    level_t *level
) {
    if (levels_library[level->blueprint_id]) {
        int size_x = LVL_size_x(level);
        int size_y = LVL_size_y(level);

        for (int x=0; x<size_x; x++) {
            for (int y=0; y<size_y; y++) {
                TILE_free(level->structure[y * size_x + x]);
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

