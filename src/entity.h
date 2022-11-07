#include "assets.h"
#include "light.h"
#include "global.h"
#include "primitives.h"
#include "sprites.h"

#ifndef ENTITY_H
#define ENTITY_H

#define MAX_VEL               5
#define MOVE_POWUH            2
#define X_FRICTION            1
#define Y_FRICTION            1
#define JUMP_POWUH            10
#define MAX_UPDATE_FUN_NUMBER 8

const static int NOTHING_FLAG    = 0;
const static int MOVABLE         = 1 << 0;  // velocity will affect entity 
const static int APPLY_COLLISION = 1 << 1;  // entity will collide with obstacles
const static int CONTROLABLE     = 1 << 2;  // entity can be controlled by player (hero)
const static int HOLDABLE        = 1 << 3;  // this entity can be held by another entity (and use its coords)
const static int EMMIT_LIGHT     = 1 << 4;  // this entity emitts light
const static int STATEABLE       = 1 << 5;  // this entity can change its states (items cannot)
const static int APPLY_FRICTION  = 1 << 6;  // this entity is affected by friction
const static int ANIMATIABLE     = 1 << 7;  // this entity has any animation beside one image
const static int NOT_DRAWABLE    = 1 << 8;  // this entity should not be drawn

enum ENITY_IDX {
    ENTITY_HERO,
    ENTITY_LANTERN,
    ENTITY_ALL
};

enum HANDLE_TYPE {
    HANDLE_LEFT_UP,
    HANDLE_LEFT_MIDDLE,
    HANDLE_LEFT_DOWN,
    HANDLE_MIDDLE_UP,
    HANDLE_MIDDLE_MIDDLE,
    HANDLE_MIDDLE_DOWN,
    HANDLE_RIGHT_UP,
    HANDLE_RIGHT_MIDDLE,
    HANDLE_RIGHT_DOWN,
    HANDLE_TYPE_ALL
};

enum ENTITY_PARAM {
    ENTITY_PARAM_SPRITE,
    ENTITY_PARAM_ANIMATION,
    ENTITY_PARAM_FLAGS,
    ENTITY_PARAM_HANDLE_TYPE,
    ENTITY_PARAM_ALL,
};

extern int entities_library[ENTITY_ALL][ENTITY_PARAM_ALL];

typedef struct entity {
    int id;                                                         // entity global id
    int state;                                                      // entity state
    int x;                                                          // position on global map
    int y;                                                          // position on global map
    int direction;                                                  // direction enity is directed
    int frame;                                                      // current animation frame index
    int frame_t;                                                    // frame timer, if big enough frame is changed
    int x_vel;                                                      // velocity in x direction
    int y_vel;                                                      // velocity in y direction
    int handle;                                                     // way of calculating handle pos
    char flags;                                                     // entity flags (8 bits)
    int update_fun_t;                                               // number of updating function
    int resolution_fun_t;                                           // number of updating function
    void(*update_fun[MAX_UPDATE_FUN_NUMBER])(struct entity *e);     // update functions
    void(*resolution_fun[MAX_UPDATE_FUN_NUMBER])(struct entity *e); // resolution functions 
    struct entity *hold;                                            // entity holded by this entity
    animation_sheet_t* sprites;                                     // entity sprites
    lightsource_t *light;                                           // entity lightsource type
} entity_t;

entity_t* ENT_init(int x, int y, animation_sheet_t* animation, texture_t* texture, int flags, int handle);
entity_t* ENT_generate(int x, int y, int id);

void ENT_update(entity_t* entity);
void ENT_update_collision(entity_t *entity, segment_t *obstacles);
void ENT_resolve(entity_t* entity);

void ENT_draw(entity_t *entity, int x, int y);
void ENT_free(entity_t *entity);

int ENT_hold_x(entity_t *entity);
int ENT_hold_y(entity_t *entity);

#endif
