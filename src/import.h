#include "global.h"
#include "tile.h"
#include "gfx.h"

#ifndef IMPORT_H
#define IMPORT_H

enum readState {
    IDLE,
    X_SIZE_READ,
    Y_SIZE_READ, 
    TILES_SUM_READ,
    TILE_SUM_READ,
    TILE_X_READ,
    TILE_Y_READ,
    ALL_TILES_READ,
};

// struct for holding level data (temporarly)
typedef struct mapfile {
    FILE             *file;
    char             *filename;
    char             *tileset_filename;
    int               state;
} mapfile_t;

mapfile_t* IMP_new_mapfile(char *filename);

level_t* IMP_read_from_file(char* filename);
int IMP_read_file(mapfile_t *map);
int IMP_read_tileset(level_t *level, mapfile_t *map);
void IMP_free_mapfile(mapfile_t *map);

#endif
