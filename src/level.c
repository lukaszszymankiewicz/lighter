#include "global.h"
#include "import.h"
#include "light.h"
#include "gfx.h"
#include "tile.h"
#include "entity.h"
#include "import.h"
#include "geometry.h"
#include "level.h"
#include "primitives.h"
#include "segment.h"

enum { W, S, A, D };

static char buffer[BUFFER_SIZE];
level_t           *levels[ASSET_LEVEL_ALL];

// this stores temporary tile data to determine if tile is an obstacle
typedef struct cell {
    int edge[4];
} cell_t;

const static int EMPTY_CELL = -1;

// creates new empty level struct
level_t* LVL_new() {
    level_t *new_level           = NULL;
    new_level                    = (level_t*)malloc(sizeof(level_t));

    new_level->structure         = NULL;
    new_level->obstacles         = NULL;
    new_level->obstacle_segments = NULL;

    new_level->tileset           = NULL;
    new_level->tileset           = (texture_t*)malloc(sizeof(texture_t));

    for (int i=0; i<MAX_ENTITY; i++) {
        new_level->entities[i]          = NULL;
    }

    return new_level;
}

// adds new entity to level, it should be done mainly while creating new level.
// returning -1 means that there is no space for another entity
// if id is inputted to -1, the first empty slot is used
int LVL_add_entity(level_t* level, entity_t *entity, int id) {
    if (id != -1) {
        level->entities[id] = entity;
        return id;
    }

    for (int i=0; i<MAX_ENTITY; i++) {
        if (!level->entities[i]) {
            level->entities[i] = entity;
            entity->id = i;
            return i;
        }
    }
    return -1;
}

entity_t* LVL_get_entity_by_id(level_t* level, int id) {
    return level->entities[id];
}

int LVL_hero_x(level_t* level) {
    return level->entities[ENTITY_HERO]->x;
}

int LVL_hero_y(level_t* level) {
    return level->entities[ENTITY_HERO]->y;
}

int LVL_camera_x(level_t* level) {
    return (int)SCREEN_WIDTH / 2;
}

int LVL_camera_y(level_t* level) {
    return (int)SCREEN_HEIGHT / 2;
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
    if (x<0 || x>level->size_x || y<0 || y>level->size_y) {
        return 0;
    }
    return level->obstacles[y * level->size_y + x];
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
    level_t *level
) {
    int camera_x = LVL_hero_x(level);
    int camera_y = LVL_hero_y(level);

    int st_x           = camera_x - ENTITY_DRAW_X_RANGE;
    int st_y           = camera_y - ENTITY_DRAW_Y_RANGE;
    
    int st_tile_pos_x  = st_x / TILE_WIDTH;
    int st_tile_pos_y  = st_y / TILE_HEIGHT;
    int end_tile_pos_x = st_tile_pos_x + SCREEN_TILE_PER_Y;
    int end_tile_pos_y = st_tile_pos_y + SCREEN_TILE_PER_X;

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            tile_t    *tile    = NULL;
            texture_t *texture = NULL;

            tile    = LVL_tile_on_pos(level, x, y);
            texture = LVL_get_tile_texture(level);
            
            int tile_pos_x = TILE_WIDTH  * x - st_x;
            int tile_pos_y = TILE_HEIGHT * y - st_y;
            
            if (tile != NULL) {
                GFX_render_tile(
                    texture ,
                    tile_pos_x,
                    tile_pos_y,
                    tile->x,
                    tile->y,
                    TILE_WIDTH,
                    TILE_HEIGHT
                );
            }
        }
    }
}

void LVL_fill_shadowbuffer_with_tiles(
    level_t *level,
    int      x,
    int      y,
    int      obstacle
) {
    int is_obstacle;

    int st_x           = x - SCREEN_WIDTH  / 2;
    int st_y           = y - SCREEN_HEIGHT / 2;
    int end_x          = x + SCREEN_WIDTH  / 2;
    int end_y          = y + SCREEN_HEIGHT / 2;
    
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

int LVL_fill_level(level_t *level, FILE *file) {
    int tiles_counter = 0;
    int layer_counter = 0;
    int layer_read    = 0;
    int cur_tile_idx  = 0;
    int state         = 0;
    int coords_per_single_tile_per_layer;
    int coords_per_single_tile;
    int x_size, y_size;
    int x_tile, y_tile;

    state++;

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {
      int dec_value = IMP_cast_val_to_dec(buffer);

      switch (state) {

        case READ_LEVEL_PREAMBULE_FIRST_HALF:
            if(dec_value != LEVEL_PREAMBULE[FIRST_HALF]) { return 0; }
            state++; break;

        case READ_LEVEL_PREAMBULE_SECOND_HALF:
            if(dec_value != LEVEL_PREAMBULE[SECOND_HALF]) { return 0; }
            state++; break;

        case READ_LEVEL_X_SIZE:
            x_size = dec_value;
            state++; break;

        case READ_LEVEL_Y_SIZE:
            y_size = dec_value;
            LVL_set_size(level, x_size, y_size);
            state++; break;

        case READ_LEVEL_TILES_SUM:
            coords_per_single_tile_per_layer = dec_value;
            state++;
            break;

        case READ_LEVEL_TILE_SUM:
            coords_per_single_tile = dec_value;
            state++;
            break;

        case READ_LEVEL_TILE_X:
            x_tile = dec_value;
            tiles_counter++;
            layer_counter++;
            state++;
            break;

        case READ_LEVEL_TILE_Y:
            y_tile = dec_value;
            tiles_counter++;
            layer_counter++;
            
            // both coords are read - Tile is ready to be placed somewhere on level
            if (layer_read == 0) {
                LVL_fill_structure(level, x_tile, y_tile, cur_tile_idx);
            } else if (layer_read == 1) {
                // TODO: this is temporary - it will be valid only for one type of obstacle
                // this 1 is means obstacle type
                LVL_fill_obstacle(level, x_tile, y_tile, 1);
            } else {
                LVL_add_entity(level, ENT_generate(x_tile, y_tile, ENTITY_HERO), 0);
                level->entities[0]->state = STANDING;
                level->entities[0]->hold = ENT_generate(x_tile, y_tile, ENTITY_LANTERN);
            }

            if (layer_counter == coords_per_single_tile_per_layer) {
                // current tile on this layer is done - proceed to next tile
                layer_read++;

                if (layer_read == 3) {
                    // eveything is read - move on 
                    state= READ_LEVEL_ALL_TILES_READ;
                    break;
                }

                state = READ_LEVEL_TILES_SUM;
                layer_counter = 0;
                cur_tile_idx = 0;
                tiles_counter=0;
                break;
            } else if (tiles_counter == coords_per_single_tile) {
                cur_tile_idx++;
                state = READ_LEVEL_TILE_SUM;
                tiles_counter = 0;
                break;
            // current tile is not yet read
            } else {
                state = READ_LEVEL_TILE_X;
                break;
            }
        case READ_LEVEL_ALL_TILES_READ:
            break;
      }
    }
    return 1;
}

level_t* LVL_read_level(
    const char *filename
) {
    level_t *level     = NULL;
    level              = LVL_new();

    FILE *file         = NULL;
    char *data_path    = NULL;
    char *img_path     = NULL;
    texture_t *tileset = NULL;

    data_path = IMP_concatenate_string(filename, SEPARATOR, LEVEL_STRUCTURE_SUFFIX);
    img_path  = IMP_concatenate_string(filename, SEPARATOR, LEVEL_TILESET_SUFFIX);

    file = fopen(data_path, LEVEL_READ_MODE);
    if (file == NULL) { return NULL; }

    tileset = GFX_read_texture(img_path);
    if (!tileset) { return NULL; }
    
    level->tileset = tileset;
    LVL_fill_tiles(level);

    if (LVL_fill_level(level, file) == 0) { return NULL; }
    LVL_analyze(level);

    return level;
}

void LVL_apply_collision(level_t* level, entity_t *entity) {
    // collision needs to be applied - lets get only the relatable collision
    segment_t *obs = NULL;
    segment_t *ptr = NULL;

    int r_x1 = entity->x - COLLISION_UPDATE_X_RANGE;
    int r_y1 = entity->y - COLLISION_UPDATE_Y_RANGE;
    int r_x2 = entity->x + COLLISION_UPDATE_X_RANGE;
    int r_y2 = entity->y + COLLISION_UPDATE_Y_RANGE;

    ptr = level->obstacle_segments;

    // get only the nearby obstacles to lower the cost of collision detection
    while(ptr) {
        if (GEO_seg_in_rect(
                ptr->x1, ptr->y1,
                ptr->x2, ptr->y2, 
                r_x1, r_y1,
                r_x2, r_y2 != -1)
            )
        SEG_push(
            &obs,
            ptr->x1, ptr->y1,
            ptr->x2, ptr->y2 
        );
            ptr=ptr->next;
    }

    // collisions
    if (obs) {
        ENT_update_collision(entity, obs); 
    }

    SEG_free(obs);
    obs = NULL;
}

bool LVL_entity_in_range(
    level_t *level,
    entity_t *entity,
    int range_x,
    int range_y
) {
    int hero_x = LVL_hero_x(level); 
    int hero_y = LVL_hero_y(level); 

    return GEO_pt_in_rect(
        entity->x,
        entity->y,
        hero_x - range_x,
        hero_y - range_y,
        hero_x + range_x,
        hero_y + range_y
    );
}

void LVL_update_entities(level_t *level) {
    for (int i=0; i<MAX_ENTITY; i++) {
        if (!level->entities[i]) {
            continue;
        }

        if (LVL_entity_in_range(level, level->entities[i], ENTITY_UPDATE_X_RANGE, ENTITY_UPDATE_X_RANGE)) {
            ENT_update(level->entities[i]); 

            if (level->entities[i]->flags & APPLY_COLLISION) {
                LVL_apply_collision(level, level->entities[i]);
            }

            ENT_resolve(level->entities[i]);
        }
    }
}

void LVL_update_light(level_t *level) {
    for (int i=0; i<MAX_ENTITY; i++) {
        if (!level->entities[i]) {
            continue;
        }

        if (LVL_entity_in_range(
                level,
                level->entities[i],
                ENTITY_UPDATE_LIGHT_X_RANGE,
                ENTITY_UPDATE_LIGHT_Y_RANGE
            )) {

            if (level->entities[i]->flags & EMMIT_LIGHT) {

                LIG_fill_lightbuffer(
                    level->entities[i]->x,
                    level->entities[i]->y,
                    level->entities[i]->light,
                    level->obstacle_segments
                );
            }

            if (level->entities[i]->hold != NULL && (level->entities[i]->hold->flags & EMMIT_LIGHT)) {

                LIG_fill_lightbuffer(
                    level->entities[i]->hold->x,
                    level->entities[i]->hold->y,
                    level->entities[i]->hold->light,
                    level->obstacle_segments
                );

            }
        }
    }
}

// position of entity on screen is known from the start, all the other entities are drawn by
// relation to this hero position.
void LVL_draw_entities(level_t *level) {
    int camera_x = LVL_camera_x(level);
    int camera_y = LVL_camera_y(level);
    int hero_x   = LVL_hero_x(level);
    int hero_y   = LVL_hero_y(level);

    for (int i=0; i<MAX_ENTITY; i++) {
        if (!level->entities[i]) {
            continue;
        }

        if (LVL_entity_in_range(level, level->entities[i], ENTITY_DRAW_X_RANGE, ENTITY_DRAW_Y_RANGE)) {
            int entity_x = level->entities[i]->x;
            int entity_y = level->entities[i]->y;
            int draw_x = camera_x - (hero_x - entity_x);
            int draw_y = camera_y - (hero_y - entity_y);

            ENT_draw(level->entities[i], draw_x, draw_y);

            if (level->entities[i]->hold) {

                int hold_entity_x = level->entities[i]->hold->x;
                int hold_entity_y = level->entities[i]->hold->y;
                int hold_draw_x = camera_x - (hero_x - hold_entity_x);
                int hold_draw_y = camera_y - (hero_y - hold_entity_y);

                ENT_draw(level->entities[i]->hold, hold_draw_x, hold_draw_y);
            }
        }
    }
}

void LVL_free(
    level_t *level
) {
    free(level);
    level = NULL;
}

