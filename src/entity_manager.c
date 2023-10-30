#include <stdio.h>

#include "entity.h"
#include "components.h"
#include "entity_manager.h"
#include "geometry.h"
#include "global.h"
#include "light.h"
#include "segment.h"

void ENTMAN_init(
) {
    entity_manager = NULL;
    entity_manager = (entity_manager_t*)malloc(sizeof(entity_manager_t));

    for (int i=0; i<MAX_ENTITY; i++) {
        entity_manager->entities[i]   = NULL;
    }
}

void ENTMAN_add_entity(
    int               x_tile,
    int               y_tile,
    int               id
) {
    entity_t* entity = NULL;
    // TODO: make x and y already calculatedc
    entity           = ENT_generate(x_tile*TILE_WIDTH, y_tile*TILE_HEIGHT, id);

    for (int i=0; i<MAX_ENTITY; i++) {
        if (entity_manager->entities[i] == NULL) {
            entity_manager->entities[i] = entity;
            return;
        }
    }
}

entity_t* ENTMAN_get(
    int id
) {
    return entity_manager->entities[id];
}

// TODO: make it parametrizable
int ENTMAN_hero_x(
) {
    return entity_manager->entities[ENTITY_HERO]->x;
}

int ENTMAN_hero_y(
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
    entity_t*         entity
) {
    return (ENTMAN_entity_in_range(entity, ENTITY_UPDATE_X_RANGE, ENTITY_UPDATE_X_RANGE));
}

bool ENTMAN_entity_in_light_update_range(
    entity_t*         entity
) {
    return (ENTMAN_entity_in_range(entity, ENTITY_UPDATE_LIGHT_X_RANGE, ENTITY_UPDATE_LIGHT_Y_RANGE));
}

bool ENTMAN_entity_in_draw_range(
    entity_t*         entity
) {
    return (ENTMAN_entity_in_range(entity, ENTITY_UPDATE_DRAW_X_RANGE, ENTITY_UPDATE_DRAW_Y_RANGE));
}

void ENTMAT_update(
) {
    segment_t* obstacles = level_manager->level->obstacle_segments;

    for (int i=0; i<MAX_ENTITY; i++) {
        
        entity_t* entity = NULL;
        entity           = entity_manager->entities[i];

        if (!entity) { continue; }

        if (ENTMAN_entity_in_update_range(entity)) {
            ENTMAN_apply_collision(obstacles, entity);
            ENT_update(entity); 
            ENT_resolve(entity);
        }
    }
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
    entity_t*         entity,
    segment_t*        obstacles
) {
    if (entity == NULL || ENT_has_not_flag(entity, EMMIT_LIGHT)) {
        return; 
    }

    segment_t* obs       = NULL;
    obs                  = ENTMAN_light_obstacles(entity, obstacles);
        
    lightsource_t* light = NULL;
    light                = ENT_get_light(entity);

    int emit_x           = ENT_light_emit_x(entity);
    int emit_y           = ENT_light_emit_y(entity);

    float angle          = ENT_light_angle(entity);
    float wobble_corr    = ENT_wobble_coef(entity);
    
    // TODO: add all light polygons
    // for (int i=0; i<light->n_poly; i++) {
    for (int i=0; i<1; i++) {
        LIG_draw_polygon(emit_x, emit_y, i, angle, wobble_corr, light, obs);
    }

    if (obs) { SEG_free(obs); }
}

void ENTMAN_calc_light(
) {
    segment_t* obstacles = level_manager->level->obstacle_segments;

    // for (int i=0; i<MAX_ENTITY; i++) {
    for (int i=0; i<1; i++) {
        entity_t* entity = NULL;
        entity           = entity_manager->entities[i];

        if (!entity) { continue; }

        if (ENTMAN_entity_in_light_update_range(entity)) {
            ENTMAN_calc_single_entity_light(entity, obstacles);
            ENTMAN_calc_single_entity_light(entity->hold, obstacles);
        }
    }
}

void ENTMAN_draw(
) {
    for (int i=0; i<MAX_ENTITY; i++) {

        entity_t* entity = NULL;
        entity           = entity_manager->entities[i];

        if (!entity) { continue; }

        if (ENTMAN_entity_in_draw_range(entity)) {
            ENT_draw(entity);

            if (entity->hold) {
                ENT_draw(entity->hold);
            }
        }
    }
}

void ENTMAN_free(
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
