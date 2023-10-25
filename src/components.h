#include "controller.h"
#include "entity_manager.h"
#include "level_manager.h"
#include "scene.h"
#include "timer.h"

#ifndef COMPONENTS_H
#define COMPONENTS_H

extern entity_manager_t *entity_manager;
extern level_manager_t  *level_manager;
extern controller_t     *keyboard;
extern scene_t          *scene;
extern game_timer_t     *fps_timer;
extern game_timer_t     *cap_timer;

void COMPONENTS_init();

#endif
