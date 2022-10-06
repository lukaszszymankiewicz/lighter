#include "global.h"
#include "tile.h"
#include "gfx.h"
#include "sprites.h"

#ifndef IMPORT_H
#define IMPORT_H

enum READ_STATE_LEVEL {
    IDLE,
    X_SIZE_READ,
    Y_SIZE_READ, 
    TILES_SUM_READ,
    TILE_SUM_READ,
    TILE_X_READ,
    TILE_Y_READ,
    ALL_TILES_READ,
};

enum READ_STATE_ANIMATION {
    READ_PREAMBULE_IDLE,
    READ_PREAMBULE_READING_FIRST_HALF,
    READ_PREAMBULE_READING_SECOND_HALF,
    READ_ANIMATION_READING,
};

level_t* IMP_read_level(char* filename);
animation_sheet_t* IMP_read_animation(char *img_path, char *data_path);

#endif
