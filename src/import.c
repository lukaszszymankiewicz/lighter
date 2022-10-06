#include "global.h"
#include "gfx.h"
#include "tile.h"
#include "level.h"
#include "primitives.h"
#include "import.h"
#include "segment.h"
#include "sprites.h"
#include "string.h"

static const int DOUBLE_BYTE              = 2;
static const int QUAD_BYTE                = 4;
static const int NO_READ_ERROR            = 1;
static const int ANIMATION_PREAMBULE[2]   = { 16718, 18765 };
static const char *LEVEL_READ_MODE        = "rb";
static const char *LEVEL_STRUCTURE_SUFFIX = "level.llv";
static const char *LEVEL_TILESET_SUFFIX   = "level.png";
static const char *SEPARATOR              = "/";

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

int LIG_cast_val_to_dec(char vals[DOUBLE_BYTE]) {
    return vals[0]<<8 | vals[1];
}

int IMP_fill_level(level_t *level, FILE *file) {
    const int BUFFER_SIZE = 2;
    char buffer[BUFFER_SIZE];

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

    while((fread(buffer, DOUBLE_BYTE, 1, file) == NO_READ_ERROR)) {
      int dec_value = LIG_cast_val_to_dec(buffer);

      switch (state) {
        case X_SIZE_READ:
            x_size = dec_value;
            state++;
            break;

        case Y_SIZE_READ: // x and y size of level is read - we can assign them to actual level
            y_size = dec_value;
            state++;
            LVL_set_size(level, x_size, y_size);
            break;

        case TILES_SUM_READ:
            coords_per_single_tile_per_layer = dec_value;
            state++;
            break;

        case TILE_SUM_READ:
            coords_per_single_tile = dec_value;
            state++;
            break;

        case TILE_X_READ:
            x_tile = dec_value;
            tiles_counter++;
            layer_counter++;
            state++;
            break;

        case TILE_Y_READ:
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
                level->hero_x = x_tile; 
                level->hero_y = y_tile-1; 
            }

            if (layer_counter == coords_per_single_tile_per_layer) {
                // current tile on this layer is done - proceed to next tile
                layer_read++;

                if (layer_read == 3) {
                    // eveything is read - move on 
                    state=ALL_TILES_READ;
                    break;
                }

                layer_counter = 0;
                state = TILES_SUM_READ;
                cur_tile_idx = 0;
                tiles_counter=0;
                break;
            } else if (tiles_counter == coords_per_single_tile) {
                cur_tile_idx++;
                state = TILE_SUM_READ;
                tiles_counter = 0;
                break;
            // current tile is not yet read
            } else {
                state = TILE_X_READ;
                break;
            }
        case ALL_TILES_READ:
            break;
      }
    }
    return 1;
}

level_t* IMP_read_level(
    char *filename
) {
    level_t *level = NULL;
    level          = LVL_new();

    FILE *file         = NULL;
    char *data_path    = NULL;
    char *img_path     = NULL;
    texture_t *tileset = NULL;

    data_path = IMP_concatenate_string(filename, SEPARATOR, LEVEL_STRUCTURE_SUFFIX);
    img_path = IMP_concatenate_string(filename, SEPARATOR, LEVEL_TILESET_SUFFIX);

    file = fopen(data_path, LEVEL_READ_MODE);
    if (file == NULL) { return NULL; }

    tileset = GFX_read_texture(img_path);
    if (!tileset) { return NULL; }

    LVL_fill_tiles(level);
    IMP_fill_level(level, file);

    return level;
}

animation_sheet_t* IMP_read_animation(
    char *img_path,
    char *data_path
) {
    const int BUFFER_SIZE = 2;
    char buffer[BUFFER_SIZE];

    animation_sheet_t *sheet = NULL;
    sheet                    = malloc(sizeof(animation_sheet_t));
    FILE *file               = NULL;

    int state                = READ_PREAMBULE_IDLE;
    file                     = fopen(data_path, LEVEL_READ_MODE);
    int val;
    
    state++;

    while((fread(buffer, 2, 1, file) == 1)) {

      // printf("%d %d \n", buffer[0], buffer[1]);
      // val = buffer[0]<<8 | buffer[1];
      // printf("value = %d \n", val);
      // printf("%d \n", buffer[0]);
      // printf("%d %d %d %d \n", buffer[0], buffer[1], buffer[2], buffer[3]);

      switch (state) 
      {
         case READ_PREAMBULE_READING_FIRST_HALF:
             val = LIG_cast_val_to_dec(buffer);
             if(val != ANIMATION_PREAMBULE[0]) { return NULL; }

             state++;
             break;
             
         case READ_PREAMBULE_READING_SECOND_HALF:
             val = LIG_cast_val_to_dec(buffer);
             if(val != ANIMATION_PREAMBULE[1]) { return NULL; }

             state++;
             break;

         case READ_ANIMATION_READING:
             break;
        }
    }

    return sheet;
}

