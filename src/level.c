#include <stdio.h>
#include "global.h"
#include "gfx.h"
#include "tile.h"
#include "level.h"
#include "primitives.h"
#include "segment.h"

#define SCREEN_TILE_PER_X 10
#define SCREEN_TILE_PER_Y 12

enum { W, S, A, D };

// this stores temporary tile data to determine if tile is an obstacle
typedef struct cell {
    int edge[4];
} cell_t;

// this matrix store cells - which are struct holding if tile is an obstacle. We limit number of
// such cells only to a number of those which fits on screen. As tile has 32x32 pixels and game
// screen is 320X240 matrix of 240/32=7.5 x 320/32=10. Adding some reserve 10x12 cells seems enough.
cell_t cells[SCREEN_TILE_PER_X][SCREEN_TILE_PER_Y];

// creates new empty level struct
level_t* LVL_new() {
    level_t *new_level           = NULL;
    new_level                    = (level_t*)malloc(sizeof(level_t));

    new_level->tileset           = NULL;
    new_level->structure         = NULL;
    new_level->obstacles         = NULL;
    new_level->obstacle_segments = NULL;
    new_level->tiles_used        = 0;

    new_level->tileset = malloc(sizeof(texture_t));

    return new_level;
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

// initialise level structure
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

    level->structure = malloc(sizeof (tile_t*) * level->size_x * level->size_y);
    LVL_clean_structure(level);

    level->obstacles = malloc(sizeof (int)    * level->size_x * level->size_y);
    LVL_clean_obstacles(level);
}

// allocs memory for tiles
void LVL_set_tile_number(
    level_t *level,
    int      tile_number
) {
    level->tile_array = malloc(tile_number * sizeof(tile_t));
}

// adds tile to tile array of level
void LVL_add_tile(
    level_t *level,
    int      tile_index,
    int      x_offset,
    int      y_offset
) {
    tile_t* tile                  = NULL;
    tile                          = TILE_new(x_offset, y_offset);
    level->tile_array[tile_index] = *tile;
}

void LVL_set_tileset(level_t *level, texture_t* texture) {
    level->tileset = texture;
}

// fills level structure with one tile
void LVL_fill_structure(
    level_t *level,
    int      x,
    int      y,
    int      i
) {
    level->structure[y * level->size_x + x] = &(level->tile_array[i]);
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

void LVL_fill_tiles(level_t *level) {
    // allocating memory for all tiles
    int tile_number = (int)(level->tileset->width/TILE_WIDTH) * (int)(level->tileset->height/TILE_HEIGHT); 
    LVL_set_tile_number(level, tile_number);

    // creating actual tiles
    int i = 0;

    for (int y=0; y<level->tileset->height; y+=TILE_WIDTH) {
        for (int x=0; x<level->tileset->width; x+=TILE_WIDTH) {
            LVL_add_tile(level, i, x, y);
            i++;
        }
    }
}

void MAP_get_tileset_path(
    char *tileset_filename,
    char *level_file_path
) {
    strcat(level_file_path, CURRENT_FOLDER);
    strcat(level_file_path, TILESET_FOLDER);
    strcat(level_file_path, tileset_filename);
}


tile_t* LVL_tile_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    if (x>0 && x<level->size_x && y>0 && y<level->size_y) {
        return level->structure[y * level->size_x + x];
    }
    return &level->tile_array[EMPTY_TILE];
}

int LVL_obstacle_on_pos(level_t* level, int x, int y) {
    return level->obstacles[y * level->size_x + x];
}

texture_t* LVL_get_tile_texture(
    level_t *level
) {
    return level->tileset;
}

void LVL_analyze(
    level_t *level,
    int      hero_x,
    int      hero_y
) {

    level->obstacle_segments = NULL;  // just to be sure
    segment_t *ptr           = NULL;

    // cleaning
    for (int yy = 0; yy < SCREEN_TILE_PER_X; yy++) {
        for (int xx = 0; xx < SCREEN_TILE_PER_Y; xx++) {
            cells[yy][xx].edge[W] = 0;
            cells[yy][xx].edge[S] = 0;
            cells[yy][xx].edge[A] = 0;
            cells[yy][xx].edge[D] = 0;
        }
    }

    // viewport coords
    int st_x       = hero_x - SCREEN_WIDTH/2;
    int st_y       = hero_y - SCREEN_HEIGHT/2;
    int end_x      = hero_x + SCREEN_WIDTH/2;
    int end_y      = hero_y + SCREEN_HEIGHT/2;
    
    int index          = 0;
    int st_tile_pos_x  = st_x / TILE_WIDTH;
    int st_tile_pos_y  = st_y / TILE_HEIGHT;

    int end_tile_pos_x = end_x / TILE_WIDTH + 1;
    int end_tile_pos_y = end_y / TILE_HEIGHT + 1;

    // level border
    SEG_push(&level->obstacle_segments, 0,            0,             SCREEN_WIDTH, 0             );
    SEG_push(&level->obstacle_segments, 0,            SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT );
    SEG_push(&level->obstacle_segments, 0,            0,             0,            SCREEN_HEIGHT );
    SEG_push(&level->obstacle_segments, SCREEN_WIDTH, 0,             SCREEN_WIDTH, SCREEN_HEIGHT );

    int obstacle_num = SEG_len(level->obstacle_segments);

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            // TODO: after implementing framebuffer this wont render tile as fill such buffer.
            int tile_pos_x = TILE_WIDTH  * x-st_x;
            int tile_pos_y = TILE_HEIGHT * y-st_y;

            // tiles currently on screen has coordinates shifted from absolute tile coords, so shitf
            // is made. This allows to ommit second loop only for collecting obstacles.
            int obs_x = x-st_tile_pos_x;
            int obs_y = y-st_tile_pos_y;

            // ANALYZING OBSTACLES
            if (LVL_obstacle_on_pos(level, x, y)) {

                // check LEFT border
                if (!LVL_obstacle_on_pos(level, x-1, y)) {
                    // create new
                    if (cells[obs_y-1][obs_x].edge[A] == 0) {
                        SEG_push(
                          &level->obstacle_segments,
                          tile_pos_x,
                          tile_pos_y,
                          tile_pos_x,
                          tile_pos_y + TILE_HEIGHT
                        );

                        cells[obs_y][obs_x].edge[A] = obstacle_num;

                        obstacle_num++;
                    }

                    // borrow from up
                    else {
                        index = cells[obs_y-1][obs_x].edge[A];
                        cells[obs_y][obs_x].edge[A] = index;
                        ptr = SEG_get(level->obstacle_segments, index);
                        ptr->y2 += TILE_HEIGHT;
                    }
                }

                // pass
                if (!LVL_obstacle_on_pos(level, x+1, y)) {
                    // create new
                    if (cells[obs_y-1][obs_x].edge[D] == 0) {
                        SEG_push(
                          &level->obstacle_segments,
                          tile_pos_x + TILE_WIDTH,
                          tile_pos_y,
                          tile_pos_x + TILE_WIDTH,
                          tile_pos_y + TILE_HEIGHT
                        );

                        cells[obs_y][obs_x].edge[D] = obstacle_num;
                        obstacle_num++;
                    }

                    // borrow from up
                    else {
                        index = cells[obs_y-1][obs_x].edge[D];
                        cells[obs_y][obs_x].edge[D] = index;
                        ptr = SEG_get(level->obstacle_segments, index);
                        ptr->y2 += TILE_HEIGHT;
                    }
                }

                // check UP line
                if (!LVL_obstacle_on_pos(level, x, y-1)) {
                    // create new
                    if (cells[obs_y][obs_x-1].edge[W] == 0) {
                        SEG_push(
                          &level->obstacle_segments,
                          tile_pos_x,
                          tile_pos_y,
                          tile_pos_x + TILE_WIDTH,
                          tile_pos_y
                        );

                        cells[obs_y][obs_x].edge[W] = obstacle_num;
                        obstacle_num++;
                    }

                    // borrow from left
                    else {
                        index = cells[obs_y][obs_x-1].edge[W];
                        cells[obs_y][obs_x].edge[W] = index;
                        ptr = SEG_get(level->obstacle_segments, index);
                        ptr->x2 += TILE_WIDTH;
                    }
                }

                // check BOTTOM line
                if (!LVL_obstacle_on_pos(level, x, y+1)) {
                    // create new
                    if (cells[obs_y][obs_x-1].edge[S] == 0) {
                        SEG_push(
                          &level->obstacle_segments,
                          tile_pos_x,
                          tile_pos_y + TILE_HEIGHT,
                          tile_pos_x + TILE_WIDTH,
                          tile_pos_y + TILE_HEIGHT
                        );

                        cells[obs_y][obs_x].edge[S] = obstacle_num;
                        obstacle_num++;
                    }

                    // borrow from left
                    else {
                        index = cells[obs_y][obs_x-1].edge[S];
                        cells[obs_y][obs_x].edge[S] = index;
                        ptr = SEG_get(level->obstacle_segments, index);
                        ptr->x2 += TILE_WIDTH;
                    }
                }
            }
        }
    }
}


// checks curent tile to draw, and calculates obstacles from it
void LVL_draw(
    level_t *level,
    int      hero_x,
    int      hero_y
) {

    level->obstacle_segments = NULL;  // just to be sure

    int st_x           = hero_x - SCREEN_WIDTH  / 2;
    int st_y           = hero_y - SCREEN_HEIGHT / 2;
    int end_x          = hero_x + SCREEN_WIDTH  / 2;
    int end_y          = hero_y + SCREEN_HEIGHT / 2;
    
    int st_tile_pos_x  = st_x / TILE_WIDTH;
    int st_tile_pos_y  = st_y / TILE_HEIGHT;
    int end_tile_pos_x = end_x / TILE_WIDTH + 1;
    int end_tile_pos_y = end_y / TILE_HEIGHT + 1;

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            tile_t    *tile    = NULL;
            texture_t *texture = NULL;

            tile    = LVL_tile_on_pos(level, x, y);
            texture = LVL_get_tile_texture(level);
            
            int tile_pos_x = TILE_WIDTH  * x-st_x;
            int tile_pos_y = TILE_HEIGHT * y-st_y;
            
            if (tile != NULL) {
                GFX_render_tile(
                        texture ,
                        tile_pos_x, tile_pos_y,
                        tile->x, tile->y,
                        TILE_WIDTH, TILE_HEIGHT
                    );
            }
        }
    }
}

void LVL_fill_shadowbuffer_with_tiles(
    level_t *level,
    int      hero_x,
    int      hero_y,
    int      obstacle
) {
    int is_obstacle;

    int st_x           = hero_x - SCREEN_WIDTH  / 2;
    int st_y           = hero_y - SCREEN_HEIGHT / 2;
    int end_x          = hero_x + SCREEN_WIDTH  / 2;
    int end_y          = hero_y + SCREEN_HEIGHT / 2;
    
    int st_tile_pos_x  = st_x / TILE_WIDTH;
    int st_tile_pos_y  = st_y / TILE_HEIGHT;
    int end_tile_pos_x = end_x / TILE_WIDTH + 1;
    int end_tile_pos_y = end_y / TILE_HEIGHT + 1;

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            is_obstacle        = LVL_obstacle_on_pos(level, x, y);
            
            int tile_pos_x = TILE_WIDTH  * x-st_x;
            int tile_pos_y = TILE_HEIGHT * y-st_y;

            // this will allow to decide where shadow on walls should be drawn
            if (is_obstacle == obstacle) {
                GFX_fill_rect(
                    GFX_fill_shadowbuffer,
                    tile_pos_x,
                    tile_pos_y,
                    TILE_WIDTH,
                    TILE_HEIGHT,
                    255, 255, 255, 0 
                );
            }
        }
    }
}

// TODO: test this one!
void LVL_free(
    level_t *level
) {
    free(level->obstacles);

    GFX_free_texture(level->tileset);

    free(level->tile_array);
    free(level->structure);
    free(level);
}
