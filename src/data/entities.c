#include "library.h"

#include "../animation.h"

entity_blueprint_t* entity_library[ENTITY_ALL] = {NULL};


entity_blueprint_t entity_blueprint_hero = {
    ENTITY_HERO, // id
    MOVABLE | APPLY_COLLISION | CONTROLABLE | STATEABLE | APPLY_FRICTION | ANIMATIABLE, // flags
    HANDLE_TYPE_NO,  // handle_type
    HANDLE_TYPE_NO,  // light_emmit_pt
    SPRITE_HERO, // texture_id
    LIGHTSOURCE_NO, // lighsource
    ENTITY_LIGHTER,  // hold id
    STANDING,  // starting_state
    {
        SPRITE_HERO, 4, 27, 40,
        {
            { // STANDING
                2, // len
                WOBBLE_WALKING,
                {
                    {
                        (SDL_Rect)       { 0,   0,   9,        20        },
                        { (SDL_Rect)     { 0,   0,   9,        20        } },
                        9, 10, 1, 20
                    },
                    {
                        (SDL_Rect)       { 9,        0,   9,         20        },
                        { (SDL_Rect)     { 0,        0,   9,         20        } },
                        9, 10, 1, 20
                    },
                }
            },
            { // WALKING
                2, // len
                WOBBLE_STABLE,
                {
                    {
                        (SDL_Rect)       { 0,   20,        9,        20        },
                        { (SDL_Rect)     { 0,   0,         9,        20        } },
                        9, 10, 1, 5
                    },
                    {
                        (SDL_Rect)       { 9,        20,        9,         20        },
                        { (SDL_Rect)     { 0,        0,         9,         20        } },
                        9, 10, 1, 5
                    },
                }
            },
            { // JUMPING
                1, // len
                WOBBLE_NO,
                {
                    {
                        (SDL_Rect)       { 9,        20,        9,         20        },
                        { (SDL_Rect)     { 0,        0,         9,         20        } },
                        9, 10, 1, 0
                    },
                }
            },
            { // FALLING
                1, // len
                WOBBLE_NO,
                {
                    {
                        (SDL_Rect)       { 18,        20,         9,         20        },
                        { (SDL_Rect)     { 0,         0,         9,         20        } },
                        9, 10, 1, 0
                    },
                }
            }
        }
    }
};

entity_blueprint_t entity_blueprint_lighter = {
    ENTITY_LIGHTER,
    HOLDABLE | EMMIT_LIGHT,
    HANDLE_MIDDLE_UP,
    HANDLE_FRONT_MIDDLE,
    SPRITE_LIGHTER,
    LIGHTSOURCE_LIGHTER,
    ENTITY_NO,
    NOTHING,
    {
        SPRITE_LIGHTER, 0, 10, 5, { } 
    }
};

entity_blueprint_t entity_blueprint_walllight = {
    ENTITY_WALLLIGHT,
    EMMIT_LIGHT,
    HANDLE_MIDDLE_MIDDLE,
    HANDLE_MIDDLE_MIDDLE,
    SPRITE_WALLLIGHT,
    LIGHTSOURCE_WALLLIGHT,
    ENTITY_NO,
    NOTHING,
    {
        SPRITE_WALLLIGHT, 0, 32, 32, { } 
    }
};

void LIB_read_entity_blueprint(
    entity_blueprint_t* blueprint
) {
    if (blueprint) {
        printf("ENTITY BLUEPRINT ID = %d read\n", blueprint->id);
    }
    entity_library[blueprint->id]      = blueprint;
};

void LIB_create_entity_library(
) {
    LIB_read_entity_blueprint(&entity_blueprint_hero);
    LIB_read_entity_blueprint(&entity_blueprint_lighter);
    LIB_read_entity_blueprint(&entity_blueprint_walllight);
}
