#include "library.h"

#include "../level.h"

#define PROPER_PACK_COUNT   1
#define BUFFER_SIZE         2
#define DOUBLE_BYTE         2
#define COORDS_PER_RECT     4
#define PREAMBULE_LEN       2

#define FIRST_HALF          0
#define SECOND_HALF         1

const char *LEVEL_READ_MODE              = "rb";
const int LEVEL_PREAMBULE[PREAMBULE_LEN] = { 19525, 22092 };
static char buffer[BUFFER_SIZE];

level_t* levels_library[LEVEL_ALL] = {NULL};

enum READ_LEVEL {
    READ_LEVEL_IDLE,
    READ_LEVEL_PREAMBULE_FIRST_HALF,
    READ_LEVEL_PREAMBULE_SECOND_HALF,
    READ_LEVEL_X_SIZE,
    READ_LEVEL_Y_SIZE, 
    READ_LEVEL_TILES_SUM,
    READ_LEVEL_TILES_ID,
    READ_LEVEL_TILE_SUM,
    READ_LEVEL_TILE_X,
    READ_LEVEL_TILE_Y,
    READ_LEVEL_ALL_TILES_READ,
};

level_blueprint_t level_blueprint_sample = {
    "./src/data/levels/sample/level.llv",
    "./src/data/levels/sample/level.png",
};


unsigned short int LIB_cast_val_to_dec(char vals[DOUBLE_BYTE]) {
    return (unsigned short int) ((unsigned short int) vals[0]<<8 | (unsigned char) vals[1]);
}

level_t* LIB_read_level_data(
    level_blueprint_t blueprint,
    FILE             *file 
) {

    texture_t *tileset = NULL;
    level_t* level     = NULL;
    level              = LVL_new();

    tileset = TXTR_read_from_file(blueprint.texture_path);

    int tiles_counter = 0;
    int layer_counter = 0;
    int layer_read    = 0;
    int cur_tile_idx  = 0;
    int cur_tile_id   = 0;
    int state         = 0;
    int coords_per_single_tile_per_layer;
    int coords_per_single_tile;
    int x_size, y_size;
    int x_tile, y_tile;

    state++;

    // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
    // FILE *f = fopen("level_read_log.txt", "w");

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {
      int dec_value = LIB_cast_val_to_dec(buffer);

      switch (state) {

        case READ_LEVEL_PREAMBULE_FIRST_HALF:
            if(dec_value != LEVEL_PREAMBULE[FIRST_HALF]) {
                return NULL; 
            }
            state++; break;

        case READ_LEVEL_PREAMBULE_SECOND_HALF:
            if(dec_value != LEVEL_PREAMBULE[SECOND_HALF]) {
                return NULL; 
            }
            state++; break;

        case READ_LEVEL_X_SIZE:
            x_size = dec_value;
            state++; break;

        case READ_LEVEL_Y_SIZE:
            y_size = dec_value;
            LVL_set_size(level, x_size, y_size);
            state++; break;

        case READ_LEVEL_TILES_SUM:
            // TODO(LG-15): all comments should will part of "DEBUG READ LEVEL" mode
            // fprintf(f, "detected tile per layer %d, n=%d \n", layer_read, (int)dec_value/2);
            coords_per_single_tile_per_layer = dec_value;
            state++;
            break;

        case READ_LEVEL_TILES_ID:
            cur_tile_id = dec_value;
            // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
            // fprintf(f, "detected tile with index id=%d\n", cur_tile_id);
            state++;
            break;

        case READ_LEVEL_TILE_SUM:
            // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
            // fprintf(f, "    n=%d per single tile\n", (int)dec_value/2);
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
                // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
                // fprintf(f, "        tile put on x=%d y=%d \n",  x_tile, y_tile);
                LVL_fill_structure(level, x_tile, y_tile, cur_tile_idx);
            } else if (layer_read == 1) {
                // TODO (LG-15): this is temporary - it will be valid only for one type of obstacle
                // this 1 is means obstacle type
                // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
                // fprintf(f, "        obstacle with index: 1 put on %d %d \n", x_tile, y_tile);
                LVL_fill_obstacle(level, x_tile, y_tile, 1);

            } else {
                // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
                // fprintf(f, "        entity with index: %d put on %d %d on \n", cur_tile_id, x_tile, y_tile);
                LVL_add_entity_fill(level, x_tile, y_tile, cur_tile_id);
            }

            if (layer_counter == coords_per_single_tile_per_layer) {
                // current tile on this layer is done - proceed to next tile
                layer_read++;

                if (layer_read == 3) {
                    // eveything is read - move on 
                    state = READ_LEVEL_ALL_TILES_READ;
                    break;
                }

                state = READ_LEVEL_TILES_SUM;
                layer_counter = 0;
                cur_tile_idx  = 0;
                cur_tile_id   = 0;
                tiles_counter = 0;
                break;
            } else if (tiles_counter == coords_per_single_tile) {
                cur_tile_idx++;
                state = READ_LEVEL_TILES_ID;
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
    // TODO (LG-15): all comments should will part of "DEBUG READ LEVEL" mode
    // fclose(f);
    level->tileset = tileset;

    return level;
}

level_t* LIB_read_level(
    level_blueprint_t blueprint
) {
    level_t *level        = NULL;
    FILE    *file         = NULL;
    file                  = fopen(blueprint.data_path, LEVEL_READ_MODE);
    
    if (file == NULL) {
        printf("level file cannot be read! \n");
        return NULL;
    }

    level                 = LIB_read_level_data(blueprint, file);

    if (level == NULL) {
        printf("level file cannot be interpreted! \n");
        return NULL;
    }

    LVL_analyze(level);

    fclose(file);

    return level;
}

void LIB_create_levels_library(
) {
    levels_library[LEVEL_SAMPLE] = LIB_read_level(level_blueprint_sample);
};

