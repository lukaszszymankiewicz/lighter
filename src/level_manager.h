#include "level.h"

#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

typedef struct level_manager {
    level_t      *level;
    int           id;
} level_manager_t;

void LVLMAN_init();
void LVLMAN_fill(int id);
void LVLMAN_put_to_scene();
void LVLMAN_free();

#endif
