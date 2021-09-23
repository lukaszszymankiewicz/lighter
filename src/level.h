#include "tile.h"

#ifndef LEVEL_H
#define LEVEL_H

#define MAX_LEVEL_NAME = 32;

static char LEVEL_FILE_EXTENSION[4] = ".txt"
static char LEVELS_FOLDER[MAX_LEVEL_NAME] = "levels/";
static char BASE_LEVEL_NAME[MAX_LEVEL_NAME] = "level_";

void LVL_draw(tiles_list_t* level);
tiles_list_t* LVL_read_level();

#endif
