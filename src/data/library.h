#include "../global.h"
#include "../animation.h"

#ifndef LIBRARY_H
#define LIBRARY_H

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

const static int ENTITY_NO       = -1;
const static int SEP             = 69420;
const static int END             = 5318008;

enum ENITY_IDX {
    ENTITY_HERO,
    ENTITY_LIGHTER,
    ENTITY_WALLLIGHT,
    ENTITY_ALL
};

enum ANIMATION_IDX {
    ANIMATION_STANDING,
    ANIMATION_WALKING,
    ANIMATON_JUMPING,
    ANIMATON_FALLING_DOWN,
    ANIMATON_UNUSED_1,
    ANIMATON_UNUSED_2,
    ANIMATON_UNUSED_3,
    ANIMATON_UNUSED_4,
    ANIMATON_UNUSED_5,
    ANIMATON_UNUSED_6,
    ANIMATION_MAX
};

enum HANDLE_TYPE {
    HANDLE_BACK_UP,
    HANDLE_BACK_MIDDLE,
    HANDLE_BACK_DOWN,
    HANDLE_MIDDLE_UP,
    HANDLE_MIDDLE_MIDDLE,
    HANDLE_MIDDLE_DOWN,
    HANDLE_FRONT_UP,
    HANDLE_FRONT_MIDDLE,
    HANDLE_FRONT_DOWN,
    HANDLE_TYPE_NO
};

typedef struct entity_blueprint {
    int  id;
    int  flags;
    int  handle_type;
    int  light_emmit_pt;
    int  texture_id; 
    int  lightsource_id; 
    int  hold_id; 
    int  starting_state; 
    animation_sheet_t animation;
} entity_blueprint_t;

extern entity_blueprint_t* entity_library[ENTITY_ALL];

void LIB_create_entity_library();

#endif

