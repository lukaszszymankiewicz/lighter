#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "data/library.h"

#include "gl_util.h"
#include "global.h"
#include "level.h"
#include "mat.h"
#include "scene.h"
#include "segment.h"
#include "tile.h"

#define ENTITY_RENDER_COUNT      4
#define LEVEL_VERTICES_FOR_SCENE 2500

// camera position in pixels
int   icamera_x      = 0;
int   icamera_y      = 0;

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
    return y * LVL_size_x(level) + x;
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

void LVL_fill_structure(
    level_t *level,
    int      id,
    int      row, int      col,
    int      x,   int      y
) {
    tile_t *tile    = NULL;

    int tileset_id  = tiles_library[id]->tileset_id;  
    int pos_x       = x * TILE_WIDTH;  
    int pos_y       = y * TILE_HEIGHT;  

    tile = TILE_new(id, tileset_id, row, col, pos_x, pos_y);
    LVL_set_tile(level, tile, x, y);
}

void LVL_fill(
    level_t* level
) {
    LVL_clean_structure(level);

    for (int x=0; x<LVL_size_x(level); x++) {
        for (int y=0; y<LVL_size_y(level); y++) {

            int id  = LVL_tile_blueprint_on_pos(level, x, y);
            int row = tiles_library[id]->row;
            int col = tiles_library[id]->col;

            LVL_fill_structure(level, id, row, col, x, y);
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

    int tileset_id = levels_library[blueprint_id]->tileset_id;
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

bool LVL_obstacle_on_pos(
    level_t* level,
    int x,
    int y
) {
    if (x<0 || x>LVL_size_x(level) || y<0 || y>LVL_size_y(level)) {
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

float* LVL_tiles_vertices(
    level_t *level,
    int     *i
) {
    int st_x           = icamera_x - ENTITY_DRAW_X_RANGE;
    int st_y           = icamera_y - ENTITY_DRAW_Y_RANGE;

    int st_tile_pos_x  = st_x / TILE_WIDTH;
    int st_tile_pos_y  = st_y / TILE_HEIGHT;
    int end_tile_pos_x = st_tile_pos_x + SCREEN_TILE_PER_Y;
    int end_tile_pos_y = st_tile_pos_y + SCREEN_TILE_PER_X;

    float *v = (float*)malloc(sizeof(float) * LEVEL_VERTICES_FOR_SCENE);

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            tile_t *tile = NULL;
            tile         = LVL_tile_on_pos(level, x, y);
            
            // TODO: this and method in the ENT_vertices can be commonized
            if (tile) {
                   // Top-left
                   v[(*i)++]=tile->render.x1; v[(*i)++]=tile->render.y2;
                   v[(*i)++]=tile->clip.x1;   v[(*i)++]=tile->clip.y2;
                   // Top-right
                   v[(*i)++]=tile->render.x2; v[(*i)++]=tile->render.y2;
                   v[(*i)++]=tile->clip.x2;   v[(*i)++]=tile->clip.y2; 
                   // Bottom-right
                   v[(*i)++]=tile->render.x2; v[(*i)++]=tile->render.y1;
                   v[(*i)++]=tile->clip.x2;   v[(*i)++]=tile->clip.y1;
                   // Top-left
                   v[(*i)++]=tile->render.x1; v[(*i)++]=tile->render.y2;
                   v[(*i)++]=tile->clip.x1;   v[(*i)++]=tile->clip.y2;
                   // Bottom-right
                   v[(*i)++]=tile->render.x2; v[(*i)++]=tile->render.y1;
                   v[(*i)++]=tile->clip.x2;   v[(*i)++]=tile->clip.y1;
                   // Bottom-left
                   v[(*i)++]=tile->render.x1; v[(*i)++]=tile->render.y1;
                   v[(*i)++]=tile->clip.x1;   v[(*i)++]=tile->clip.y1;
            }
        }

    }
    return v;
}

void LVL_draw(
    level_t *level
) {
    int len         = 0;
    float *vertices = NULL;
    vertices        = LVL_tiles_vertices(level, &len);
    int tileset     = LVL_tileset_id(level);
    
    array_t camera_arr  = GL_UTIL_camera();
    array_t scale_arr   = GL_UTIL_scale();
    array_t texture_arr = GL_UTIL_id(tileset);

    SCENE_activate_layer(LAYER_TILE);
    SCENE_add_new_drawable_object();
    SCENE_add_uniform(camera_arr);
    SCENE_add_uniform(scale_arr);
    SCENE_set_texture(texture_arr);
    SCENE_add_vertices(len, vertices, ENTITY_RENDER_COUNT);
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

