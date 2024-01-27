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
    // TODO: make x and y already calculated
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

bool ENTMAN_entity_in_range(
    entity_t*         entity,
    int               range_x,
    int               range_y
) {
    int hero_x = ENTMAN_entity_follow_x(ENTITY_HERO);
    int hero_y = ENTMAN_entity_follow_y(ENTITY_HERO); 

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

void ENTMAN_put_entity_light_to_scene(
    entity_t*         entity,
    segment_t*        obstacles
) {
    if (entity == NULL || ENT_has_not_flag(entity, EMMIT_LIGHT)) {
        return; 
    }

    segment_t* obs          = NULL;
    obs                     = ENTMAN_light_obstacles(entity, obstacles);
        
    lightsource_t* source   = NULL;
    source                  = ENT_get_light(entity);

    int   emit_x            = ENT_light_emit_x(entity);
    int   emit_y            = ENT_light_emit_y(entity);
    float angle             = ENT_light_angle(entity);
    float wobble_angle_corr = ENT_wobble_coef(entity);
    
    for (int i=0; i<source->n_poly; i++) {
        vertex_t *light_polygon  = NULL;
        float *coords            = NULL;
        int    n_vertices        = 0;
        int    final_x           = emit_x + SRC_get_light_polygon_x_corr(source, angle, i);
        int    final_y           = emit_y + SRC_get_light_polygon_y_corr(source, angle, i);
        float  final_width       = source->width + SRC_get_light_polygon_width_corr(source, i);
        float  final_angle       = angle + SRC_get_light_polygon_angle_corr(source, i) + wobble_angle_corr;

        light_polygon     = LIG_calculate(final_x, final_y, final_width, final_angle, obstacles);
        n_vertices        = VRTX_len(light_polygon);
        coords            = VRTX_to_coords(light_polygon);
        array_t *vertices = SCENE_polygon_coord_to_matrix(coords, n_vertices);

        SCENE_put_polygon_to_scene(
            vertices,
            final_x,
            final_y,
            source->light_polygons[i].red,
            source->light_polygons[i].green,
            source->light_polygons[i].blue,
            source->light_polygons[i].light_power
        );
    }

    // TODO: this should not bee done here I think
    if (obs) { SEG_free(obs); }
}

int ENTMAN_entity_follow_x(
    int entity_id
) {
    entity_t *entity = entity_manager->entities[entity_id];
    int w  = ENT_current_frame_width(entity);
    int x  = entity_manager->entities[entity_id]->x;

    return x + w/2 - SCREEN_WIDTH/2;
}

int ENTMAN_entity_follow_y(
    int entity_id
) {
    entity_t *entity = entity_manager->entities[entity_id];
    int h  = ENT_current_frame_height(entity);
    int y  = entity_manager->entities[entity_id]->y;

    return y + h/2 - SCREEN_HEIGHT/2;
}

void ENTMAN_put_light_to_scene(
) {
    segment_t* obstacles = level_manager->level->obstacle_segments;
    
    for (int i=0; i<MAX_ENTITY; i++) {
        entity_t* entity = NULL;
        entity           = entity_manager->entities[i];

        if (!entity) {
            continue; 
        }

        if (ENTMAN_entity_in_light_update_range(entity)) {
            ENTMAN_put_entity_light_to_scene(entity, obstacles);
            ENTMAN_put_entity_light_to_scene(entity->hold, obstacles);
        }
    }
}

void ENTMAN_put_to_scene(
) {
    for (int i=0; i<MAX_ENTITY; i++) {
        entity_t* entity = NULL;
        entity           = entity_manager->entities[i];

        if (!entity) {
            continue; 
        }

        if (ENTMAN_entity_in_draw_range(entity)) {
            ENT_put_to_scene(entity);

            if (entity->hold) {
                ENT_put_to_scene(entity->hold);
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
