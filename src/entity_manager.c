#include <stdio.h>

#include "entity.h"
#include "entity_manager.h"
#include "geometry.h"
#include "global.h"
#include "light.h"
#include "segment.h"
#include "scene.h"

entity_manager_t* ENTMAN_new(
) {
    entity_manager_t* entity_manager  = NULL;
    entity_manager                    = (entity_manager_t*)malloc(sizeof(entity_manager_t));

    for (int i=0; i<MAX_ENTITY; i++) {
        entity_manager->entities[i]   = NULL;
    }

    return entity_manager;
}

void ENTMAN_add_entity(
    entity_manager_t* entity_manager,
    int               x_tile,
    int               y_tile,
    int               id
) {
    entity_t* entity = NULL;
    entity           = ENT_generate(x_tile*TILE_WIDTH, y_tile*TILE_HEIGHT, id);

    for (int i=0; i<MAX_ENTITY; i++) {
        if (entity_manager->entities[i] == NULL) {
            entity_manager->entities[i] = entity;
            return;
        }
    }
}

entity_t* ENTMAN_get(
    entity_manager_t* entity_manager,
    int id
) {
    return entity_manager->entities[id];
}

int ENTMAN_hero_x(
    entity_manager_t* entity_manager
) {
    return entity_manager->entities[ENTITY_HERO]->x;
}

int ENTMAN_hero_y(
    entity_manager_t* entity_manager
) {
    return entity_manager->entities[ENTITY_HERO]->y;
}

void ENTMAN_apply_collision(
    segment_t* obstacles,
    entity_t *entity
) {
    if (ENT_has_not_flag(entity, APPLY_COLLISION)) {
        return;
    }

    segment_t *obs = NULL;

    obs = SEG_filter_by_rect(
        obstacles,
        entity->x - COLLISION_UPDATE_X_RANGE,
        entity->y - COLLISION_UPDATE_Y_RANGE,
        entity->x + COLLISION_UPDATE_X_RANGE,
        entity->y + COLLISION_UPDATE_Y_RANGE
    );

    // collisions
    if (obs) {
        ENT_update_collision(entity, obs); 
    }

    if (obs) {
        SEG_free(obs);
        obs = NULL;
    }
}

// TODO: we only check one point of entity whether is inside rect, this can not be sufficient for
// smaller ranges
// TODO: calc middle point of entity to be sure that if fits into check range
bool ENTMAN_entity_in_range(
    entity_manager_t* entity_manager,
    entity_t*         entity,
    int               range_x,
    int               range_y
) {
    int hero_x = ENTMAN_hero_x(entity_manager); 
    int hero_y = ENTMAN_hero_y(entity_manager); 

    return GEO_pt_in_rect(
        entity->x,
        entity->y,
        hero_x - range_x,
        hero_y - range_y,
        hero_x + range_x,
        hero_y + range_y
    );
}

bool ENTMAN_entity_in_update_range(
    entity_manager_t* entity_manager,
    entity_t*         entity
) {
    return (ENTMAN_entity_in_range(entity_manager, entity, ENTITY_UPDATE_X_RANGE, ENTITY_UPDATE_X_RANGE));
}

bool ENTMAN_entity_in_light_update_range(
    entity_manager_t* entity_manager,
    entity_t*         entity
) {
    return (ENTMAN_entity_in_range(entity_manager, entity, ENTITY_UPDATE_LIGHT_X_RANGE, ENTITY_UPDATE_LIGHT_Y_RANGE));
}

bool ENTMAN_entity_in_draw_range(
    entity_manager_t* entity_manager,
    entity_t*         entity
) {
    return (ENTMAN_entity_in_range(entity_manager, entity, ENTITY_UPDATE_DRAW_X_RANGE, ENTITY_UPDATE_DRAW_Y_RANGE));
}

void ENTMAT_update(
    entity_manager_t* entity_manager,
    segment_t*        obstacles
) {
    for (int i=0; i<MAX_ENTITY; i++) {
        
        entity_t* entity = NULL;
        entity           = entity_manager->entities[i];

        if (!entity) { continue; }

        if (ENTMAN_entity_in_update_range(entity_manager, entity)) {
            ENTMAN_apply_collision(obstacles, entity);
            ENT_update(entity); 
            ENT_resolve(entity);
        }
    }
}

int ENTMAN_camera_corr_by_hero_pos_x(
    entity_manager_t* entity_manager
) {
    return CAMERA_X - ENTMAN_hero_x(entity_manager);
}

int ENTMAN_camera_corr_by_hero_pos_y(
    entity_manager_t* entity_manager
) {
    return CAMERA_Y - ENTMAN_hero_y(entity_manager);
}

int ENTMAN_entity_draw_x(
    entity_manager_t* entity_manager,
    entity_t*         entity
) {
    return CAMERA_X - (ENTMAN_hero_x(entity_manager) - entity->x);
}

int ENTMAN_entity_draw_y(
    entity_manager_t* entity_manager,
    entity_t*         entity
) {
    return CAMERA_Y - (ENTMAN_hero_y(entity_manager) - entity->y);
}

segment_t* ENTMAN_light_obstacles(
    entity_t*  entity,
    segment_t* obstacles
) {
    segment_t *obs = NULL;
    
    if (ENT_has_flag(entity, EMMIT_LIGHT)) {
        obs = SEG_filter_by_rect(
            obstacles,
            entity->x - ENTITY_UPDATE_LIGHT_X_RANGE,
            entity->y - ENTITY_UPDATE_LIGHT_Y_RANGE,
            entity->x + ENTITY_UPDATE_LIGHT_X_RANGE,
            entity->y + ENTITY_UPDATE_LIGHT_Y_RANGE
        );
    }

    return obs;
}

void ENTMAN_calc_single_entity_light(
    entity_manager_t* entity_manager,
    entity_t*         entity,
    segment_t*        obstacles
) {
    if (entity == NULL || ENT_has_not_flag(entity, EMMIT_LIGHT)) {
        return; 
    }

    segment_t* obs = NULL;
    obs            = ENTMAN_light_obstacles(entity, obstacles);
        
    lightsource_t* light = NULL;
    light                = ENT_get_light(entity);

    int emit_x = ENT_light_emit_x(entity);
    int emit_y = ENT_light_emit_y(entity);

    float angle       = ENT_light_angle(entity);
    float wobble_corr = ENT_wobble_coef( entity);

    int rel_x = CAMERA_X - ENTMAN_hero_x(entity_manager);
    int rel_y = CAMERA_Y - ENTMAN_hero_y(entity_manager);
    
    int n_poly = light->n_poly;
    
    if (entity->id != ENTITY_LIGHTER) { return; }

    // for (int i=0; i<n_poly; i++) {
    for (int i=0; i<1; i++) {
        LIG_add_to_scene(
            emit_x, emit_y,
            rel_x,  rel_y,
            i, angle, wobble_corr, light, obs
        );
    }

    if (obs) { SEG_free(obs); }
}

void ENTMAN_calc_light(
    entity_manager_t* entity_manager,
    segment_t*        obstacles
) {
    for (int i=0; i<MAX_ENTITY; i++) {
        entity_t* entity = NULL;
        entity           = entity_manager->entities[i];

        if (!entity) { continue; }

        if (ENTMAN_entity_in_light_update_range(entity_manager, entity)) {
            ENTMAN_calc_single_entity_light(entity_manager, entity, obstacles);
            ENTMAN_calc_single_entity_light(entity_manager, entity->hold, obstacles);
        }
    }
}

void ENTMAN_put_on_scene(
    entity_manager_t* entity_manager
) {
    int camera_x = ENTMAN_hero_x(entity_manager);
    int camera_y = ENTMAN_hero_y(entity_manager);

    for (int i=0; i<MAX_ENTITY; i++) {

        entity_t* entity = NULL;
        entity           = entity_manager->entities[i];

        if (!entity) { continue; }

        if (ENTMAN_entity_in_draw_range(entity_manager, entity)) {
            ENT_add_to_scene(entity, camera_x, camera_y);

            if (entity->hold) {
                ENT_add_to_scene(entity->hold, camera_x, camera_y);
            }
        }
    }
}

void ENTMAN_free(
    entity_manager_t* entity_manager
) { 
    for (int i=0; i<MAX_ENTITY; i++) {
        if (entity_manager->entities[i] == NULL) {
            continue;
        }

        ENT_free(entity_manager->entities[i]);
        entity_manager->entities[i] = NULL;
    }

    free(entity_manager);
    entity_manager = NULL;
}
