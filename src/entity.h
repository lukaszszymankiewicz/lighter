#include "assets.h"
#include "source.h"
#include "global.h"
#include "primitives.h"
#include "animation.h"
#include "texture.h"
#include "data/library.h"

#ifndef ENTITY_H
#define ENTITY_H

#define MAX_VEL               5
#define MOVE_POWUH            2
#define X_FRICTION            1
#define Y_FRICTION            1
#define JUMP_POWUH            10
#define MAX_UPDATE_FUN_NUMBER 8

typedef struct entity {
    // global
    int id;                                                         // entity global id

    // position
    int x;                                                          // position on global map
    int y;                                                          // position on global map
    int direction;                                                  // direction enity is directed
    int x_vel;                                                      // velocity in x direction
    int y_vel;                                                      // velocity in y direction

    // state
    int state;                                                      // entity state
    int frame;                                                      // current animation frame index
    int frame_t;                                                    // frame timer, if big enough frame is changed

    // interactions
    int handle;                                                     // way of calculating handle pos
    int light_pt;                                                   // point from where light is emitted
    char flags;                                                     // entity flags (8 bits)
    struct entity *hold;                                            // entity holded by this entity

    // actions
    int update_fun_t;                                               // number of updating function
    int resolution_fun_t;                                           // number of resolve function
    void(*update_fun[MAX_UPDATE_FUN_NUMBER])(struct entity *e);     // update functions
    void(*resolution_fun[MAX_UPDATE_FUN_NUMBER])(struct entity *e); // resolution functions 

    // TODO: add render rect function dependent if ANIMATIABLE (ENT_texture_render replecament)

    // assets
    animation_sheet_t *sheet;                                       // entity sprites
    lightsource_t     *light;                                       // entity lightsource type

} entity_t;

entity_t* ENT_init(
    int                  x,
    int                  y,
    entity_blueprint_t*  blueprint
);

entity_t* ENT_generate(int x, int y, int id);

void ENT_update(entity_t* entity);
void ENT_update_collision(entity_t *entity, segment_t *obstacles);
void ENT_resolve(entity_t* entity);

void ENT_draw(entity_t *entity, int x, int y);
void ENT_free(entity_t *entity);
void ENT_update_hold(entity_t *entity);

int ENT_get_x(entity_t *entity);
int ENT_get_y(entity_t *entity);
int ENT_hold_x(entity_t *entity);
int ENT_hold_y(entity_t *entity);
int ENT_handle_x(entity_t *entity);
int ENT_handle_y(entity_t *entity);

int ENT_light_pt_x(entity_t *entity);
int ENT_light_pt_y(entity_t *entity);

int ENT_light_emit_x(entity_t *entity);
int ENT_light_emit_y(entity_t *entity);

int ENT_held_item_x(entity_t *entity);
int ENT_held_item_y(entity_t *entity);

lightsource_t* ENT_get_light(entity_t *entity);

#endif
