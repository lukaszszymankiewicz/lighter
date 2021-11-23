#include <stdio.h>
#include "config.h"
#include "gfx.h"
#include "tile.h"
#include "level.h"
#include "primitives.h"
#include "segment.h"

#define ENDLINE ';'
#define NEWFIELD ','

enum { W, S, A, D };

// this stores temporary tile data to determine if tile is an obstacle
typedef struct cell2 {
    int edge[4];
} cell_t;

cell_t cells[10][12];

mapfile_t* MAP_new(
    char *filename
) {
    mapfile_t *new_mapfile = NULL;
    new_mapfile            = (mapfile_t*)malloc(sizeof(mapfile_t));

    memset(new_mapfile->line_buffer, 0, MAX_LINE_LENGHT * (sizeof(char)));

    new_mapfile->file_pointer = NULL;
    new_mapfile->file_pointer = fopen(filename, "r");

    if (new_mapfile->file_pointer == NULL) {
        printf("Failed: ");
        exit(0);
    }

    return new_mapfile;
}

level_t* LVL_new() {
    level_t *new_level           = NULL;

    new_level                    = (level_t*)malloc(sizeof(level_t));
    new_level->tileset_array     = NULL;
    new_level->tile_array        = NULL;
    new_level->structure         = NULL;
    new_level->obstacles         = NULL;
    new_level->obstacle_segments = NULL;

    return new_level;
}

void LVL_set_size(
    level_t *level,
    int      size_x,
    int      size_y
) {
    level->size_x    = size_x;
    level->size_y    = size_y;
    level->structure = malloc(sizeof (tile_t*) * level->size_x * level->size_y);
    level->obstacles = malloc(sizeof (int*)    * level->size_x * level->size_y);
}

void LVL_set_tileset_number(
    level_t *level,
    int      tileset_number
) {
    level->tileset_array = malloc(tileset_number * sizeof(texture_t*));
}

void LVL_set_tile_number(
    level_t *level,
    int      tile_number
) {
    level->tile_array = malloc(tile_number * sizeof(tile_t*));
}

void LVL_add_tile(
    level_t *level,
    int      tile_index,
    int     *data
) {
    tile_t* tile                  = NULL;
    tile                          = TILE_new(data[TILESET_INDEX], data[X_OFFSET], data[Y_OFFSET]);
    level->tile_array[tile_index] = *tile;
}

void LVL_fill_structure(
    level_t *level,
    int      x,
    int      y,
    int      tile_type
) {
    level->structure[y * level->size_x + x] = &(level->tile_array[tile_type]);
}

void LVL_fill_obstacle(
    level_t *level,
    int      x,
    int      y,
    int      value
) {
    level->obstacles[y * level->size_x + x] = value;
}

void MAP_read_next_line(
    mapfile_t* map
) {
    fgets(map->line_buffer, MAX_LINE_LENGHT - 1, map->file_pointer);
    map->line_buffer[strcspn(map->line_buffer, "\n")] = 0;
}

int MAP_find_integer_in_line(
    mapfile_t *map
) {
    int  k                       = 0;
    char buffer[MAX_INT_LEN];
    
    while(map->line_buffer[k] != ENDLINE){
        buffer[k] = map->line_buffer[k];
        k++;
    }

    MAP_read_next_line(map);

    return strtol(buffer, NULL, 10);
}

void MAP_find_str_in_line(
    mapfile_t *map,
    char       buffer[]
) {
    int k = 0;                      

    while(map->line_buffer[k] != ENDLINE){
        buffer[k] = map->line_buffer[k];
        k++;
    }
    buffer[k] = '\0';

    MAP_read_next_line(map);
}

int MAP_strings_are_equal(
    char *line_buffer,
    char *label
){
    return strcmp(label, line_buffer) == 0;
}

void MAP_find_section(
    mapfile_t *map,
    char      *label
) {
    while(!MAP_strings_are_equal(map->line_buffer, label)) {
        MAP_read_next_line(map);
    }
    MAP_read_next_line(map);
}

void MAP_get_tileset_path(
    char *tileset_filename,
    char *level_file_path
) {
    strcat(level_file_path, CURRENT_FOLDER);
    strcat(level_file_path, TILESET_FOLDER);
    strcat(level_file_path, tileset_filename);
}

void MAP_get_size_section_data(
    mapfile_t *map,
    level_t   *level
) {
    int size_x = 0;
    int size_y = 0;

    MAP_find_section(map, "SIZE");
    size_x = MAP_find_integer_in_line(map);
    size_y = MAP_find_integer_in_line(map);
    LVL_set_size(level, size_x, size_y);
}

int MAP_end_of_section(
    mapfile_t *map,
    char      *section
) {
    return !MAP_strings_are_equal(map->line_buffer, section);
}

int* MAP_find_array_in_line(
    mapfile_t *map,
    int        size
) {
    int *arr                     = NULL;
    int  k                       = 0;
    int  i                       = 0;
    int  z                       = 0;
    int  number;
    char buffer[MAX_INT_LEN];
    
    memset(buffer, 0, MAX_INT_LEN * (sizeof(char)));
    arr = (int*)malloc(sizeof(int) * size);

    while(map->line_buffer[k] != ENDLINE){
        if (map->line_buffer[k] == NEWFIELD) {
            arr[z++] = strtol(buffer, NULL, 10);
            memset(buffer, 0, MAX_INT_LEN * (sizeof(char)));
            k++;
            i=0;
        }
        buffer[i++] = map->line_buffer[k++];
    }

    number = strtol(buffer, NULL, 10);
    arr[z] = number;

    MAP_read_next_line(map);

    return arr;
}

void MAP_get_tileset_section_data(
    mapfile_t *map,
    level_t   *level
) {
    char     buffer[MAX_LINE_LENGHT];
    char     secondary_buffer[MAX_LINE_LENGHT];

    memset(buffer, 0, MAX_LINE_LENGHT * (sizeof(char)));
    memset(secondary_buffer, 0, MAX_LINE_LENGHT * (sizeof(char)));

    MAP_find_section(map, "TILESET");
    LVL_set_tileset_number(level, MAP_find_integer_in_line(map));

    for (int z=0; MAP_end_of_section(map, "ENDTILESET"); z++){
        MAP_find_str_in_line(map, buffer);
        MAP_get_tileset_path(buffer, secondary_buffer);
        level->tileset_array[z] = *(GFX_read_texture(secondary_buffer));
    }
}

void MAP_get_tile_section_data(
    mapfile_t *map,
    level_t   *level
) {
    int* tile_data = NULL;

    MAP_find_section(map, "TILE");
    LVL_set_tile_number(level, MAP_find_integer_in_line(map));

    for (int z=0; MAP_end_of_section(map, "ENDTILE"); z++){
        tile_data = MAP_find_array_in_line(map, TILE_DATA_NUM);
        LVL_add_tile(level, z, tile_data);
    }
}

void MAP_get_structure_section_data(
    mapfile_t *map,
    level_t   *level
) {
    int *row = NULL;
    int  x;
    int  y;

    MAP_find_section(map, "STRUCTURE");

    for (y=0; MAP_end_of_section(map, "ENDSTRUCTURE"); y++){
        row = NULL;
        row = MAP_find_array_in_line(map, level->size_x);

        for (x=0; x<level->size_x; x++){
            LVL_fill_structure(level, x, y, row[x]);
        }
    }
}

void MAP_get_obstacle_section_data(
    mapfile_t *map,
    level_t   *level
) {
    int *row = NULL;
    int  x;
    int  y;

    MAP_find_section(map, "OBSTACLE");

    for (y=0; MAP_end_of_section(map, "ENDOBSTACLE"); y++) {
        row = NULL;
        row = MAP_find_array_in_line(map, level->size_x);

        for (x=0; x<level->size_x; x++){
            LVL_fill_obstacle(level, x, y, row[x]);
        }
    }
}

level_t* LVL_read_from_file(
    char *filename
) {
    level_t *level = NULL;
    level          = LVL_new();
    mapfile_t* map = NULL;
    map            = MAP_new("./levels/level_1.txt"); // just for now

    MAP_get_size_section_data(map, level);
    MAP_get_tileset_section_data(map, level);
    MAP_get_tile_section_data(map, level);
    MAP_get_structure_section_data(map, level);
    MAP_get_obstacle_section_data(map, level);

    return level;
}

tile_t* LVL_tile_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    return level->structure[y * level->size_x + x];
}

int LVL_tile_index_on_pos(
    level_t *level,
    int      x,
    int      y
) {
    return level->structure[y * level->size_x + x]->index;
}

int LVL_obstacle_on_pos(level_t* level, int x, int y) {
    return level->obstacles[y * level->size_x + x];
}

texture_t* LVL_get_tile_texture(
    level_t* level,
    int      index
) {
    return &level->tileset_array[index];
}

SDL_Rect LVL_get_tile_rect(
    level_t *level,
    int      x,
    int      y
) {
    tile_t* tile  = LVL_tile_on_pos(level, x, y);
    SDL_Rect rect = {tile->x, tile->y, tile->width, tile->height};
    return rect;
}

void LVL_analyze(
    level_t *level,
    int      hero_x,
    int      hero_y
) {

    level->obstacle_segments = NULL;  // just to be sure
    segment_t *ptr           = NULL;

    // clearing
    for (int yy = 0; yy < 10; yy++) {
        for (int xx = 0; xx < 12; xx++) {
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

    // viewport coords
    int st_x       = hero_x - SCREEN_WIDTH/2;
    int st_y       = hero_y - SCREEN_HEIGHT/2;
    int end_x      = hero_x + SCREEN_WIDTH/2;
    int end_y      = hero_y + SCREEN_HEIGHT/2;
    
    int st_tile_pos_x  = st_x / TILE_WIDTH;
    int st_tile_pos_y  = st_y / TILE_HEIGHT;

    int end_tile_pos_x = end_x / TILE_WIDTH + 1;
    int end_tile_pos_y = end_y / TILE_HEIGHT + 1;

    for (int x=st_tile_pos_x; x<end_tile_pos_x; x++) {
        for (int y=st_tile_pos_y; y<end_tile_pos_y; y++) {

            tile_t* tile = LVL_tile_on_pos(level, x, y);
            texture_t* texture = LVL_get_tile_texture(level, tile->index);

            // TODO: after implementing framebuffer this wont render tile as fill such buffer.
            int tile_pos_x = TILE_WIDTH  * x-st_x;
            int tile_pos_y = TILE_HEIGHT * y-st_y;

            GFX_render_tile(
                    texture,
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

void LVL_free(
    level_t *level
) {
    free(level->obstacles);
    SEG_free(level->obstacle_segments);
    free(level->tileset_array);
    free(level->tile_array);
    free(level->structure);
}
