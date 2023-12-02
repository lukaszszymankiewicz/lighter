#include "global.h"
#include "entity.h"

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

typedef struct entity_manager {
    entity_t      *entities[MAX_ENTITY];
} entity_manager_t;

void ENTMAN_init();

void ENTMAN_add_entity(int x_tile, int y_tile, int id);
void ENTMAN_put_to_scene();
void ENTMAN_put_light_to_scene();
void ENTMAT_update();
void ENTMAN_apply_collision(segment_t* obstacles, entity_t *entity);

bool ENTMAN_entity_in_range(entity_t* entity, int range_x, int range_y);

int ENTMAN_entity_follow_x(); 
int ENTMAN_entity_follow_y(); 

void ENTMAN_free();

#endif
