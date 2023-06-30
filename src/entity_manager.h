#include "global.h"
#include "entity.h"
#include "light.h"

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

typedef struct entity_manager {
    entity_t      *entities[MAX_ENTITY];
} entity_manager_t;

entity_manager_t* ENTMAN_new();

void ENTMAN_add_entity(entity_manager_t* entity_manager, int x_tile, int y_tile, int id);
void ENTMAN_put_on_scene(entity_manager_t* entity_manager);
void ENTMAN_calc_light(entity_manager_t* entity_manager, segment_t* obstacles);
void ENTMAT_update(entity_manager_t* entity_manager, segment_t* obstacles);
void ENTMAN_apply_collision(segment_t* obstacles, entity_t *entity);

bool ENTMAN_entity_in_range(entity_manager_t* entity_manager, entity_t* entity, int range_x, int range_y);

int ENTMAN_hero_x(entity_manager_t* entity_manager); 
int ENTMAN_hero_y(entity_manager_t* entity_manager); 

void ENTMAN_free(entity_manager_t* entity_manager);

#endif
