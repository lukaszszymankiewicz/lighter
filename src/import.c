#include "global.h"
#include "level.h"
#include "files.h"
#include "primitives.h"
#include "import.h"
#include "sprites.h"
#include "string.h"

#define PROPER_PACK_COUNT   1
#define BUFFER_SIZE         2
#define DOUBLE_BYTE         2
#define COORDS_PER_RECT     4
#define PREAMBULE_LEN       2

#define FIRST_HALF          0
#define SECOND_HALF         1

static const int ANIMATION_PREAMBULE[PREAMBULE_LEN]   = { 16718, 18765 };
static const int WOBBLE_PREAMBULE[PREAMBULE_LEN]      = { 22338, 16972 };
static const int LIGHTSOURCE_PREAMBULE[PREAMBULE_LEN] = { 19539, 21059 };
static const int LEVEL_PREAMBULE[PREAMBULE_LEN]       = { 19525, 22092 };

static const char *LEVEL_READ_MODE        = "rb";
static const char *LEVEL_STRUCTURE_SUFFIX = "level.llv";
static const char *LEVEL_TILESET_SUFFIX   = "level.png";
static const char *SEPARATOR              = "/";

static char buffer[BUFFER_SIZE];

animation_sheet_t *animations[ASSET_ANIMATION_ALL];
texture_t         *gradients[ASSET_GRADIENT_ALL];
texture_t         *sprites[ASSET_SPRITE_ALL];
wobble_t          *wobbles[ASSET_WOBBLE_ALL];
level_t           *levels[ASSET_LEVEL_ALL];
lightsource_t     *lightsources[ASSET_LIGHTSOURCE_ALL]; 

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

unsigned short int IMP_cast_val_to_dec(char vals[DOUBLE_BYTE]) {
    return (unsigned short int) ((unsigned short int) vals[0]<<8 | (unsigned char) vals[1]);
}

texture_t* IMP_read_texture(
    const char *filepath
) {
	SDL_Texture *new_texture    = NULL;
    SDL_Surface *loaded_surface = NULL;

    loaded_surface              = IMG_Load(filepath);

    // dummy texture if reading file failed
    if (loaded_surface == NULL) {
        new_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 20, 20);
        texture_t* p = malloc(sizeof(texture_t));

        p->surface = new_texture;
        p->width   = loaded_surface->w;
        p->height  = loaded_surface->h;

        return p;
    }
    else {
        SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x80, 0xFF, 0xFF));
        new_texture  = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        texture_t* p = malloc(sizeof(texture_t));

        p->surface = new_texture;
        p->width   = loaded_surface->w;
        p->height  = loaded_surface->h;

        SDL_FreeSurface(loaded_surface);

        return p;
    }
};

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
                level->hero_x = x_tile; 
                level->hero_y = y_tile-1; 
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

level_t* IMP_read_level(
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

    tileset = IMP_read_texture(img_path);
    if (!tileset) { return NULL; }
    
    level->tileset = tileset;
    LVL_fill_tiles(level);

    if (IMP_fill_level(level, file) == 0) { return NULL; }

    return level;
}

animation_sheet_t* IMP_read_animation(
    const char *filepath
) {
    animation_sheet_t *sheet      = NULL;

    sheet                         = malloc(sizeof(animation_sheet_t));

    int state                     = READ_ANIMATION_PREAMBULE_IDLE;
    FILE *file                    = NULL;
    file                          = fopen(filepath, LEVEL_READ_MODE);
    
    int coords[COORDS_PER_RECT];
    int idx                       = 0; // temp container
    int frame_idx                 = 0; // temp container
    int hitbox_idx                = 0; // temp container
    int rect_coord_idx            = 0; // temp container
    int cur_animation             = 0; // temp container

    state++;

    if (!file) {return NULL;}

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {

    switch (state) 
    {
        // preambule
        case READ_ANIMATION_PREAMBULE_FIRST_HALF:
            if(IMP_cast_val_to_dec(buffer) != ANIMATION_PREAMBULE[FIRST_HALF]) { return NULL; }
            state++; break;
             
        case READ_ANIMATION_PREAMBULE_SECOND_HALF:
            if(IMP_cast_val_to_dec(buffer) != ANIMATION_PREAMBULE[SECOND_HALF]) { return NULL; }
            state++; break;
        
        // animation sheet
        case READ_ANIMATIONS_NUMBER:
            sheet->n_animations = IMP_cast_val_to_dec(buffer);
            state++; break;
        
        // single animation
        case READ_ANIMATION_IDX:
            idx = IMP_cast_val_to_dec(buffer);
            state++; break;

       case READ_ANIMATION_N_FRAMES:
            sheet->animations[idx].len = IMP_cast_val_to_dec(buffer);
            state++; break;

       // single frame
       case READ_ANIMATION_DELAY:
            sheet->animations[idx].frames[frame_idx].delay = IMP_cast_val_to_dec(buffer);
            state++; break;
       
       case READ_ANIMATION_RECT:
            coords[rect_coord_idx] = IMP_cast_val_to_dec(buffer);

            if (++rect_coord_idx == COORDS_PER_RECT) {

                sheet->animations[idx].frames[frame_idx].rect.x = coords[0];
                sheet->animations[idx].frames[frame_idx].rect.y = coords[1];
                sheet->animations[idx].frames[frame_idx].rect.w = coords[2];
                sheet->animations[idx].frames[frame_idx].rect.h = coords[3];

                rect_coord_idx = 0; state++; 
            }
            break;

       case READ_ANIMATION_HITBOX_PER_FRAME:
            sheet->animations[idx].frames[frame_idx].n_hit_box = IMP_cast_val_to_dec(buffer);
            state++; break;

       case READ_ANIMATION_HITBOX_RECT:
            coords[rect_coord_idx] = IMP_cast_val_to_dec(buffer);

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
                    }
                }
            }
            break;
        }
    }

    return sheet;
}

wobble_t* IMP_read_wobble(
    const char *filepath
) {
    wobble_t *wobble = NULL;
    wobble           = (wobble_t*)malloc(sizeof(wobble_t));

    int state                     = READ_WOBBLE_IDLE;
    FILE *file                    = NULL;
    file                          = fopen(filepath, LEVEL_READ_MODE);
     
    int coef_n                    = 0;
    int sign                      = 0;
    int val                       = 0;

    state++;

    if (!file) {return NULL;}

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {
        switch (state) 
        {
            case READ_WOBBLE_PREAMBULE_FIRST_HALF:
                if(IMP_cast_val_to_dec(buffer) != WOBBLE_PREAMBULE[FIRST_HALF]) { return NULL; }
                state++; break;

            case READ_WOBBLE_PREAMBULE_SECOND_HALF:
                if(IMP_cast_val_to_dec(buffer) != WOBBLE_PREAMBULE[SECOND_HALF]) { return NULL; }
                state++; break;

            case READ_WOBBLE_NUMBER:
                wobble->len = IMP_cast_val_to_dec(buffer);
                wobble->coefs = (float*)malloc(sizeof(float) * wobble->len);
                state++; break;

            case READ_WOBBLE_SIGN:
                val = IMP_cast_val_to_dec(buffer);
                sign = val==1 ? 1 : -1;
                state++; break;

            case READ_WOBBLE_COEF:
                val = IMP_cast_val_to_dec(buffer);
                wobble->coefs[coef_n] = (float)(sign * val) / 1000.0;

                if (++coef_n == wobble->len) {
                    state++; break;
                } else {
                    state = READ_WOBBLE_SIGN; break;
                }
        }
    }

    return wobble;
}

lightsource_t* IMP_read_lightsource(
    const char *filepath
) {
    lightsource_t *lightsource = NULL;
    lightsource                = (lightsource_t*)malloc(sizeof(lightsource_t));

    int state                   = READ_LIGHTSOURCE_IDLE;
    FILE *file                  = NULL;
    file                        = fopen(filepath, LEVEL_READ_MODE);
     
    int n_polygon               = 0;
    int width                   = 0;
    int x                       = 0;
    int y                       = 0;

    state++;

    if (!file) {return NULL;}

    while((fread(buffer, DOUBLE_BYTE, PROPER_PACK_COUNT, file) == PROPER_PACK_COUNT)) {
        switch (state) 
        {
            case READ_LIGHTSOURCE_PREAMBULE_FIRST_HALF:
                if(IMP_cast_val_to_dec(buffer) != LIGHTSOURCE_PREAMBULE[FIRST_HALF]) { return NULL; }
                state++; break;

            case READ_LIGHTSOURCE_PREAMBULE_SECOND_HALF:
                if(IMP_cast_val_to_dec(buffer) != LIGHTSOURCE_PREAMBULE[SECOND_HALF]) { return NULL; }
                state++; break;

            case READ_LIGHSOURCE_WIDTH:
                width = IMP_cast_val_to_dec(buffer);

                // just to avoid dividing zero
                if (width != 0) {
                    lightsource->width = PI / IMP_cast_val_to_dec(buffer);
                } else {
                    lightsource->width = 0.0;
                }

                state++; break;

            case READ_LIGHSOURCE_PENETRATING_POWER:
                lightsource->penetrating_power = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGONS_NUMBER:

                lightsource->n_poly = IMP_cast_val_to_dec(buffer);
                lightsource->light_polygons = NULL;
                lightsource->light_polygons = (lightpolygon_t*)malloc(sizeof(lightpolygon_t) * lightsource->n_poly);

                state++; break;

            case READ_LIGHSOURCE_POLYGON_X:
                x = IMP_cast_val_to_dec(buffer);

                // invert the sign
                if (x > 255 * 256) {
                    x -= 255 * 256;
                    x *= -1;
                }

                lightsource->light_polygons[n_polygon].x = x;
                state++; break;

            case READ_LIGHSOURCE_POLYGON_Y:
                y = IMP_cast_val_to_dec(buffer);

                // invert the sign
                if (y > 255 * 256) {
                    y -= 255 * 256;
                    y *= -1;
                }
                lightsource->light_polygons[n_polygon].y = y;
                state++; break;

            case READ_LIGHSOURCE_POLYGON_RED:
                lightsource->light_polygons[n_polygon].red = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGON_GREEN:
                lightsource->light_polygons[n_polygon].green = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGON_BLUE:
                lightsource->light_polygons[n_polygon].blue = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGON_POWER:
                lightsource->light_polygons[n_polygon].light_power = IMP_cast_val_to_dec(buffer);
                state++; break;

            case READ_LIGHSOURCE_POLYGON_WIDTH:
                lightsource->light_polygons[n_polygon].width = IMP_cast_val_to_dec(buffer);

                if (++n_polygon == lightsource->n_poly) {
                    state++; break;
                } {
                    state = READ_LIGHSOURCE_POLYGON_X; break;
                }

            case READ_LIGHSOURCE_WOBBABLE:
                lightsource->wobbable = (bool)IMP_cast_val_to_dec(buffer);
                state++; break;
        }
    }

    return lightsource;
}

void IMP_read_all_files() {
    animations[ASSET_HERO_ANIMATION]         = IMP_read_animation(FILEPATH_HERO_ANIMATION);
    gradients[ASSET_GRADIENT_CIRCULAR]       = IMP_read_texture(FILEPATH_GRADIENT_CIRCULAR);
    sprites[ASSET_SPRITE_HERO]               = IMP_read_texture(FILEPATH_SPRITE_HERO);
    wobbles[ASSET_WOBBLE_NO]                 = IMP_read_wobble(FILEPATH_WOBBLE_NO);
    wobbles[ASSET_WOBBLE_STABLE]             = IMP_read_wobble(FILEPATH_WOBBLE_STABLE);
    wobbles[ASSET_WOBBLE_WALKING]            = IMP_read_wobble(FILEPATH_WOBBLE_WALKING);
    levels[ASSET_LEVEL_SAMPLE]               = IMP_read_level(FILEPATH_LEVEL_SAMPLE);
    lightsources[ASSET_LIGHTER_LIGHTSOURCE]  = IMP_read_lightsource(FILEPATH_LIGTHER_LIGHTSOURCE);
    lightsources[ASSET_LANTERN_LIGHTSOURCE]  = IMP_read_lightsource(FILEPATH_LANTERN_LIGHTSOURCE);
}

void IMP_update_all_files() {
    lightsources[ASSET_LIGHTER_LIGHTSOURCE]->gradient = gradients[ASSET_GRADIENT_CIRCULAR];
    lightsources[ASSET_LANTERN_LIGHTSOURCE]->gradient = gradients[ASSET_GRADIENT_CIRCULAR];
}

void IMP_free_all_files() {
    ANIM_free(animations[ASSET_HERO_ANIMATION]);
    GFX_free_texture(gradients[ASSET_GRADIENT_CIRCULAR]);
    GFX_free_texture(sprites[ASSET_SPRITE_HERO]);
    LIG_free_wobble(wobbles[ASSET_WOBBLE_NO]);
    LIG_free_wobble(wobbles[ASSET_WOBBLE_STABLE]);
    LIG_free_wobble(wobbles[ASSET_WOBBLE_WALKING]);
    LVL_free(levels[ASSET_LEVEL_SAMPLE]);
    LIG_free_lightsource(lightsources[ASSET_LIGHTER_LIGHTSOURCE]);
    LIG_free_lightsource(lightsources[ASSET_LANTERN_LIGHTSOURCE]);
}
