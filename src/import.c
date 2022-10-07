#include "global.h"
#include "gfx.h"
#include "tile.h"
#include "level.h"
#include "primitives.h"
#include "import.h"
#include "segment.h"
#include "sprites.h"
#include "string.h"

#define PROPER_PACK_COUNT   1
#define BUFFER_SIZE         2
#define DOUBLE_BYTE         2
#define COORDS_PER_RECT     4

static const int ANIMATION_PREAMBULE[2]   = { 16718, 18765 };
static const char *LEVEL_READ_MODE        = "rb";
static const char *LEVEL_STRUCTURE_SUFFIX = "level.llv";
static const char *LEVEL_TILESET_SUFFIX   = "level.png";
static const char *SEPARATOR              = "/";

static char buffer[BUFFER_SIZE];

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
    animation_sheet_t *sheet      = NULL;
    sheet                         = TXTR_init_animation_sheet(img_path);

    int state                     = READ_PREAMBULE_IDLE;
    FILE *file                    = NULL;
    file                          = fopen(data_path, LEVEL_READ_MODE);
    
    int coords[COORDS_PER_RECT];
    int idx                       = 0; // temp container
    int frame_idx                 = 0; // temp container
    int hitbox_idx                = 0; // temp container
    int rect_coord_idx            = 0; // temp container
    int cur_animation             = 0; // temp container

    state++;

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {

    switch (state) 
    {
        // preambule
        case READ_PREAMBULE_READING_FIRST_HALF:
            if(LIG_cast_val_to_dec(buffer) != ANIMATION_PREAMBULE[0]) { return NULL; }
            state++; break;
             
        case READ_PREAMBULE_READING_SECOND_HALF:
            if(LIG_cast_val_to_dec(buffer) != ANIMATION_PREAMBULE[1]) { return NULL; }
            state++; break;
        
        // animation sheet
        case READ_ANIMATIONS_NUMBER:
            sheet->n_animations = LIG_cast_val_to_dec(buffer);
            state++; break;
        
        // single animation
        case READ_ANIMATION_IDX:
            printf("idx = %d \n", LIG_cast_val_to_dec(buffer));
            idx = LIG_cast_val_to_dec(buffer);
            state++; break;

       case READ_ANIMATION_N_FRAMES:
            printf("len = %d \n", LIG_cast_val_to_dec(buffer));
            sheet->animations[idx].len = LIG_cast_val_to_dec(buffer);
            state++; break;

       // single frame
       case READ_ANIMATION_DELAY:
            sheet->animations[idx].frames[frame_idx].delay = LIG_cast_val_to_dec(buffer);
            printf("delay = %d \n", LIG_cast_val_to_dec(buffer));
            state++; break;
       
       case READ_ANIMATION_RECT:
            coords[rect_coord_idx] = LIG_cast_val_to_dec(buffer);
            printf("coord = %d \n", LIG_cast_val_to_dec(buffer));

            if (++rect_coord_idx == COORDS_PER_RECT) {

                sheet->animations[idx].frames[frame_idx].rect.x = coords[0];
                sheet->animations[idx].frames[frame_idx].rect.y = coords[1];
                sheet->animations[idx].frames[frame_idx].rect.w = coords[2];
                sheet->animations[idx].frames[frame_idx].rect.h = coords[3];

                rect_coord_idx = 0; state++; 
            }
            break;

       case READ_ANIMATION_HITBOX_PER_FRAME:
            printf("hitbox per frame = %d \n", LIG_cast_val_to_dec(buffer));
            sheet->animations[idx].frames[frame_idx].n_hit_box = LIG_cast_val_to_dec(buffer);
            state++; break;

       case READ_ANIMATION_HITBOX_RECT:
            coords[rect_coord_idx] = LIG_cast_val_to_dec(buffer);
            printf("hit box coord = %d \n", LIG_cast_val_to_dec(buffer));

            if (rect_coord_idx++ == COORDS_PER_RECT-1) {
                sheet->animations[idx].frames[frame_idx].hit_boxes[hitbox_idx].x = coords[0];
                sheet->animations[idx].frames[frame_idx].hit_boxes[hitbox_idx].y = coords[1];
                sheet->animations[idx].frames[frame_idx].hit_boxes[hitbox_idx].w = coords[2];
                sheet->animations[idx].frames[frame_idx].hit_boxes[hitbox_idx].h = coords[3];

                rect_coord_idx = 0;

                if (++hitbox_idx == sheet->animations[idx].frames[frame_idx].n_hit_box) {

                    if (++frame_idx < sheet->animations[idx].len) {
                        state = READ_ANIMATION_DELAY;
                        hitbox_idx = 0;
                        rect_coord_idx = 0;
                    } else if (++cur_animation < sheet->n_animations) {
                        hitbox_idx = 0;
                        rect_coord_idx = 0;
                        frame_idx = 0;
                        state = READ_ANIMATION_IDX;
                    } else {
                        state++;
                        printf("im done correctly! \n");
                    }
                }
            }
            break;
        }
    }

    return sheet;
}
