#include <stdbool.h>

#include "../animation.h"
#include "../source.h"
#include "../global.h"
#include "../source.h"
#include "../level.h"

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

enum WOBBLE_IDX {
    WOBBLE_NO,
    WOBBLE_STABLE,
    WOBBLE_WALKING,
    WOBBLE_ALL
};

enum LIGHTSOURCES_IDX {
    LIGHTSOURCE_LIGHTER,
    LIGHTSOURCE_WALLLIGHT,
    LIGHTSOURCE_NO,
    LIGHTSOURCE_ALL
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

enum SPRITE_IDX {
    SPRITE_HERO,
    SPRITE_LIGHTER,
    SPRITE_WALLLIGHT,
    SPRITE_ALL
};

enum LEVEL_IDX {
    LEVEL_SAMPLE,
    LEVEL_ALL
};

enum TILESET_IDX {
    TILESET_BASIC,
    TILESET_ALL
};

enum TILE_IDX {
    TILE_WALL,
    TILE_BROKEN_WALL,
    TILE_BLACK,
    TILE_ALL
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

typedef struct texture_blueprint {
    const char *filepath;
} texture_blueprint_t;

typedef struct tile_blueprint {
    int   tileset_id;
    float x1;
    float y1;
    float x2;
    float y2;
    bool  obstacle;
} tile_blueprint_t;

typedef struct level_blueprint {
    const char *data_path;
    const char *texture_path;
} level_blueprint_t;

extern entity_blueprint_t *entity_library[ENTITY_ALL];
extern wobble_t           *wobble_library[WOBBLE_ALL];
extern lightsource_t      *lighsources_library[LIGHTSOURCE_ALL];
extern level_t            *levels_library[LEVEL_ALL];
extern texture_t          *sprites_library[SPRITE_ALL];
extern texture_t          *tilesets_library[TILESET_ALL];

void LIB_create_entity_library();
void LIB_create_wobble_library();
void LIB_create_lightsources_library();
void LIB_create_sprites_library();
void LIB_create_levels_library();
void LIB_create_tilesets_library();
void LIB_create_tile_library()

void LIB_create_all();
bool LIB_validate();
void LIB_free_all();

#endif

