#include "global.h"
#include "gfx.h"
#include "tile.h"
#include "level.h"
#include "import.h"
#include "primitives.h"
#include "segment.h"
#include "string.h"

static const int VAL_SIZE = 2;
static const int NO_READ_ERROR = 1;
static const char *LEVEL_READ_MODE = "rb";
static const char *LEVEL_STRUCTURE_PREFIX = "level.llv";
static const char *LEVEL_TILESET_PREFIX = "level.png";
static const char *SEPARATOR = "/";

char* IMP_concatenate_string(
    const char *a, const char *b, const char *d
) {
    size_t len = strlen(a) + strlen(b) + strlen(d);
    char* str = NULL;
    str = (char*)malloc(len + 1);

    strcpy(str, a);
    strcat(str, b);
    strcat(str, d); 

    return str;
}

mapfile_t* IMP_new_mapfile(
    char *filename
) {
    mapfile_t *new_mapfile         = NULL;
    new_mapfile                    = (mapfile_t*)malloc(sizeof(mapfile_t));

    new_mapfile->file              = NULL;
    new_mapfile->filename          = NULL;
    new_mapfile->tileset_filename  = NULL;
    new_mapfile->state             = IDLE;

    new_mapfile->filename = IMP_concatenate_string(filename, SEPARATOR, LEVEL_STRUCTURE_PREFIX);
    new_mapfile->tileset_filename = IMP_concatenate_string(filename, SEPARATOR, LEVEL_TILESET_PREFIX);

    return new_mapfile;
}

void IMP_free_mapfile(
    mapfile_t *map
) {
    if (map->file != NULL) {
        fclose(map->file);
    }

    if (map != NULL) {
        free(map);
        map = NULL;
    }
}

int castValToDec(unsigned char vals[VAL_SIZE]) {
    return vals[0]<<8 | vals[1];
}

int IMP_fill_level(mapfile_t *map, level_t *level) {
    unsigned char bin_val[2];

    int tiles_counter = 0;
    int layer_counter = 0;
    int layer_read = 0;
    int cur_tile_idx = 0;
    int coords_per_single_tile_per_layer;
    int coords_per_single_tile;
    int x_size, y_size;
    int x_tile, y_tile;

    map->state++;

    while((fread(bin_val, VAL_SIZE, 1, map->file) == NO_READ_ERROR)) {
      int decValue = castValToDec(bin_val);

      switch (map->state) {
        case X_SIZE_READ:
            x_size = decValue;
            map->state++;
            break;

        case Y_SIZE_READ: // x and y size of level is read - we can assign them to actual level
            y_size = decValue;
            map->state++;
            LVL_set_size(level, x_size, y_size);
            break;

        case TILES_SUM_READ:
            coords_per_single_tile_per_layer = decValue;
            map->state++;
            break;

        case TILE_SUM_READ:
            coords_per_single_tile = decValue;
            map->state++;
            break;

        case TILE_X_READ:
            x_tile = decValue;
            tiles_counter++;
            layer_counter++;
            map->state++;
            break;

        case TILE_Y_READ:
            y_tile = decValue;
            tiles_counter++;
            layer_counter++;
            
            // both coords are read - Tile is ready to be placed somewhere on level
            if (layer_read == 0) {
                LVL_fill_structure(level, x_tile, y_tile, cur_tile_idx);
            } else if (layer_read == 1) {
                // TODO: this is temporary - it will be valid only for one type of obstacle
                LVL_fill_obstacle(level, x_tile, y_tile, 1);
            } else {
                level->hero_x = x_tile; 
                level->hero_y = y_tile-1; 
            }

            if (layer_counter == coords_per_single_tile_per_layer) {
                // current tile on this layer is done - proceed to next tile
                layer_read++;

                if (layer_read == 3) {
                    // eveything is read - move on 
                    map->state=ALL_TILES_READ;
                    break;
                }

                layer_counter = 0;
                map->state = TILES_SUM_READ;
                cur_tile_idx = 0;
                tiles_counter=0;
                break;
            } else if (tiles_counter == coords_per_single_tile) {
                cur_tile_idx++;
                map->state = TILE_SUM_READ;
                tiles_counter = 0;
                break;
            // current tile is not yet read
            } else {
                map->state = TILE_X_READ;
                break;
            }
        case ALL_TILES_READ:
            break;
      }
    }

    return 1;
}

int IMP_read_file(mapfile_t *map) {
    map->file = fopen(map->filename, LEVEL_READ_MODE);

    if (map->file == NULL) {
        return -1;
    }
    return 1;
}

int IMP_read_tileset(level_t *level, mapfile_t *map) {
    texture_t *tileset = NULL;
    tileset = GFX_read_texture(map->tileset_filename);

    if (tileset == NULL) {
        return -1;
    } else if ((tileset->width % TILE_WIDTH) || (tileset->height % TILE_HEIGHT)) {
        return -1;
    }

    LVL_set_tileset(level, tileset);
    return 1;
}

level_t* IMP_read_from_file(
    char *filename
) {
    level_t *level;
    level = NULL;
    level = LVL_new();

    mapfile_t* map;
    map = NULL;
    map = IMP_new_mapfile(filename);

    if (IMP_read_file(map) == -1) {
        return NULL;
    }

    if (IMP_read_tileset(level, map) == -1) {
        return NULL;
    }

    LVL_fill_tiles(level);
    IMP_fill_level(map, level);

    IMP_free_mapfile(map);

    return level;
}

