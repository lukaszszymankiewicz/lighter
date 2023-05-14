#include <stdio.h>

#include "data/library.h"

#include "global.h"
#include "geometry.h"
#include "controller.h"
#include "entity.h"
#include "texture.h"
#include "gfx.h"
#include "primitives.h"
#include "sorted_list.h"
#include "animation.h"
#include "source.h"

int state_collisions[3][3] = {
    //  STANDING, WALKING, JUMPING
    {1,            1,         1    },
    {1,            1,         1    },
    {0,            0,         0    },
};

frame_t* ENT_current_frame(
    entity_t *entity
) {
    if (!(entity->flags & ANIMATIABLE)) {
        return NULL;
    }
    return &(entity->sheet->animations[entity->state].frames[entity->frame]);
}

SDL_Rect* ENT_current_frame_rect(
    entity_t *entity
) {
    if (!(entity->flags & ANIMATIABLE)) {
        return ANIM_get_whole_texture_size(entity->sheet);
    }

    frame_t *frame = &(entity->sheet->animations[entity->state].frames[entity->frame]);

    SDL_Rect *rect = NULL;
    rect           = (SDL_Rect*)malloc(sizeof(SDL_Rect));

    rect->x = frame->rect.x;
    rect->y = frame->rect.y;
    rect->w = frame->rect.w;
    rect->h = frame->rect.h;

    return rect;
}

int ENT_get_x(
    entity_t *entity
) {
    return entity->x;
}

int ENT_get_y(
    entity_t *entity
) {
    return entity->y;
}

// correction value from handle point to point zero of entity
int ENT_handle_x(
    entity_t *entity
) {
    SDL_Rect *rect = NULL;
    rect           = ENT_current_frame_rect(entity);
    int x          = 0;

    if (
        entity->handle == HANDLE_BACK_UP || 
        entity->handle == HANDLE_BACK_MIDDLE || 
        entity->handle == HANDLE_BACK_DOWN
    ) {
        if (entity->direction == LEFT) { x = rect->w; }
        else { x = 0; }

    } else if  (
        entity->handle == HANDLE_MIDDLE_UP || 
        entity->handle == HANDLE_MIDDLE_MIDDLE || 
        entity->handle == HANDLE_MIDDLE_DOWN
    ) {
        x = (int) rect->w / 2;
    } else {
        if (entity->direction == RIGHT) { x = rect->w; }
        else { x = 0; }
    }
    free(rect);

    return x;
}

// correction value from handle point to point zero of entity
int ENT_handle_y(
    entity_t *entity
) {
    SDL_Rect *rect = NULL;
    rect           = ENT_current_frame_rect(entity);
    int y          = 0;

    if (
        entity->handle == HANDLE_BACK_UP || 
        entity->handle == HANDLE_MIDDLE_UP || 
        entity->handle == HANDLE_FRONT_UP 
    ) {
        y = 0;
    } else if  (
        entity->handle == HANDLE_BACK_MIDDLE || 
        entity->handle == HANDLE_MIDDLE_MIDDLE || 
        entity->handle == HANDLE_FRONT_MIDDLE
    ) {
        y = (int) rect->h / 2;
    } else {
        y = rect->h;
    }

    free(rect);

    return y;
}

// where entity has "hand" and holds item, it is corr
int ENT_hold_x(
    entity_t *entity
) {
    if (!(entity->flags & ANIMATIABLE)) {
        return ENT_handle_x(entity);
    }

    frame_t* frame = NULL;
    frame          = ENT_current_frame(entity);
    int hold_x     = 0;

    if (entity->direction == RIGHT) {
        hold_x = frame->handle_x;
    } else {
        hold_x = frame->rect.w - frame->handle_x;
    }

    return hold_x;
}

// where entity has "hand" and holds item, it is corr
int ENT_hold_y(
    entity_t *entity
) {
    if (!(entity->flags & ANIMATIABLE)) {
        return ENT_handle_y(entity);
    }

    frame_t* frame = NULL;
    frame          = ENT_current_frame(entity);
    int hold_y     = frame->handle_y;

    return hold_y;
}

int ENT_held_item_x(
    entity_t *entity
) {
    return entity->x + ENT_hold_x(entity) - ENT_handle_x(entity->hold);
}

int ENT_held_item_y(
    entity_t *entity
) {
    return entity->y + ENT_hold_y(entity) - ENT_handle_y(entity->hold);
}

void ENT_set_hold_x(
    entity_t *entity
) {
    entity->hold->x = ENT_held_item_x(entity);
}

void ENT_set_hold_y(
    entity_t *entity
) {
    entity->hold->y = ENT_held_item_y(entity);
}

void ENT_update_hold(
    entity_t *entity
) {
    ENT_set_hold_x(entity);
    ENT_set_hold_y(entity);
}

int ENT_cur_animation_delay(
    entity_t *entity
) {
    return entity->sheet->animations[entity->state].frames[entity->frame].delay;
}

int ENT_cur_animation_len(
    entity_t *entity
) {
    return entity->sheet->animations[entity->state].len;
}

void ENT_update_animation(
    entity_t *entity
) {
    entity->frame_t++;
    int del = ENT_cur_animation_delay(entity);
    int len = ENT_cur_animation_len(entity);

    if (entity->frame_t >= del) {
        entity->frame_t=0;
        entity->frame++; 

        if (entity->frame >= len) {
            entity->frame=0;
        }
    }
}

bool ENT_check_state_collision(
    int current_state,
    int desired_state
) {
    return state_collisions[current_state][desired_state];
}

void ENT_update_state(
    entity_t *entity
) {
    if (entity->x_vel == 0 && ENT_check_state_collision(entity->state, STANDING)) {
        entity->state=STANDING; 
    }
    if (entity->y_vel > 0) {
        printf("entity with id %d is start to falling down \n", entity->id);
        entity->state=FALLING_DOWN; 
    }
}

void ENT_direct_holdable_upwards(
    entity_t* entity
) {
    if (!(entity)) {return ;}

    if (entity->flags & EMMIT_LIGHT) {
        SRC_move_lightsource(ENT_get_light(entity), UP, entity->direction);
    }
}

void ENT_direct_holdable_downwards(
    entity_t* entity
) {
    if (!(entity)) {return ;}

    if (entity->flags & EMMIT_LIGHT) {
        SRC_move_lightsource(ENT_get_light(entity), DOWN, entity->direction);
    }
}

void ENT_direct_holdable_clean(
    entity_t* entity,
    int cur_dir
) {
    if (!entity) { return; }

    if (entity->flags & EMMIT_LIGHT) {
        SRC_move_lightsource(ENT_get_light(entity), NONE, cur_dir);
    }
}

void ENT_change_dir_holdable(
    entity_t* entity,
    int new_direction
) {
    if (!(entity) || new_direction == entity->direction) {
        return;
    }

    entity->direction = new_direction;

    if (entity->flags & EMMIT_LIGHT) {
        SRC_move_lightsource(ENT_get_light(entity), NONE, new_direction);
    }
}

void ENT_change_dir(
    entity_t* entity,
    int new_direction
) {

    if (new_direction == entity->direction) {
        return;
    }

    entity->direction = new_direction;

    if (entity->flags & EMMIT_LIGHT) {
        SRC_move_lightsource(ENT_get_light(entity), NONE, new_direction);
    }

    ENT_change_dir_holdable(entity->hold, new_direction);
}

void ENT_update_friction(
    entity_t *entity
) {
    if (entity->direction == RIGHT) {
        entity->x_vel = MAX(0, entity->x_vel - X_FRICTION);
    }
    else {
        entity->x_vel = MIN(0, entity->x_vel + X_FRICTION);
    }

    if (entity->state == JUMPING || entity->state == FALLING_DOWN) {
        entity->y_vel += Y_FRICTION;
    }
}

void ENT_jump(
    entity_t *entity
) {
    if (entity->state == STANDING || entity->state == WALKING || entity->state == NOTHING) {
        entity->state = JUMPING;
        entity->y_vel -= JUMP_POWUH;
    }
}

void ENT_move(
    entity_t      *entity,
    direction_t  direction
) {
    if (!(entity->flags & MOVABLE)) {return;}
    
    if (entity->state == STANDING || entity->state == WALKING || entity->state == NOTHING) {
        entity->state = WALKING;

        if (entity->direction == RIGHT) {
            entity->x_vel = MIN(MAX_VEL, entity->x_vel + MOVE_POWUH);
        }
        else {
            entity->x_vel = MAX(-1*MAX_VEL, (entity->x_vel - MOVE_POWUH));
        }
    }

    else if (entity->state == JUMPING || entity->state == FALLING_DOWN) {
        if (entity->direction == RIGHT) {
            entity->x_vel = MIN(MAX_VEL, entity->x_vel + MOVE_POWUH);
        }
        else {
            entity->x_vel = MAX(-1*MAX_VEL, (entity->x_vel - MOVE_POWUH));
        }
    }
}

void ENT_update_control(
    entity_t *entity
) {
    if (CON_button_still_pressed(keyboard, SDL_SCANCODE_W)) {
        ENT_direct_holdable_upwards(entity->hold);
    }

    else if (CON_button_still_pressed(keyboard, SDL_SCANCODE_S)) {
        ENT_direct_holdable_downwards(entity->hold);
    }

    else {
        ENT_direct_holdable_clean(entity->hold, entity->direction);
    }

    if (CON_button_still_pressed(keyboard, SDL_SCANCODE_SPACE)) {
        ENT_jump(entity);
    }

    if (CON_button_still_pressed(keyboard, SDL_SCANCODE_D)) {
        ENT_move(entity, RIGHT);
        ENT_change_dir(entity, RIGHT);
    }

    if (CON_button_still_pressed(keyboard, SDL_SCANCODE_A)) {
        ENT_move(entity, LEFT);
        ENT_change_dir(entity, LEFT);
    }
}

void ENT_update_y_vel(
    entity_t *entity
) {
    entity->y += entity->y_vel;
}

void ENT_update_x_vel(
    entity_t *entity
) {
    entity->x += entity->x_vel;
}

void ENT_update_velocity(
    entity_t *entity
) {
    ENT_update_y_vel(entity);
    ENT_update_x_vel(entity);
}

void ENT_update_wobble(
    entity_t* entity
) {
    if (!entity->light) {return;}
    entity->light->frame++;
}

entity_t* ENT_init(
    int                 x, 
    int                 y,
    entity_blueprint_t* blueprint
) {
    entity_t* entity  = (entity_t*)malloc(sizeof(entity_t));

    // global
    entity->id               = blueprint->id;
    
    // position 
    entity->x                = x;        
    entity->y                = y;
    entity->direction        = RIGHT; // right is the default but I donw know why
    entity->x_vel            = 0;
    entity->y_vel            = 0;

    // state
    entity->state            = blueprint->starting_state;
    entity->frame            = 0;
    entity->frame_t          = 0;

    // interactions
    entity->handle           = blueprint->handle_type;
    entity->light_pt         = blueprint->light_emmit_pt;
    entity->flags            = blueprint->flags;
    entity->hold             = NULL;

    // assets
    entity->sheet            = &blueprint->animation;

    entity->light            = NULL;
    entity->light            = lightsources[blueprint->lightsource_id];

    // actions
    entity->update_fun_t     = 0;                
    entity->resolution_fun_t = 0;                

    if (entity->flags & CONTROLABLE) {
        entity->update_fun[entity->update_fun_t++] = ENT_update_control;
    }

    if (entity->flags & APPLY_FRICTION) {
        entity->update_fun[entity->update_fun_t++] = ENT_update_friction;
    }

    if (entity->flags & EMMIT_LIGHT) {
        entity->update_fun[entity->update_fun_t++] = ENT_update_wobble;
    }

    if (entity->flags & MOVABLE) {
        entity->resolution_fun[entity->resolution_fun_t++] = ENT_update_velocity;
        entity->resolution_fun[entity->resolution_fun_t++] = ENT_update_hold;
    }

    if (entity->flags & STATEABLE) {
        entity->resolution_fun[entity->resolution_fun_t++] = ENT_update_state;
    }

    if (entity->flags & ANIMATIABLE) {
        entity->resolution_fun[entity->resolution_fun_t++] = ENT_update_animation;
    }
    
    if (blueprint->hold_id != ENTITY_NO) {
        entity->hold = ENT_generate(entity->x, entity->y, blueprint->hold_id);
    }

    return entity;
}

void ENT_update(entity_t* entity) {
    for(int i=0; i<entity->update_fun_t; i++) {
        entity->update_fun[i](entity);
    }

    if (entity->hold) {
        ENT_update(entity->hold);
    }
}

void ENT_resolve(entity_t* entity) {
    for(int i=0; i<entity->resolution_fun_t; i++) {
        entity->resolution_fun[i](entity);
    }

    if (entity->hold) {
        ENT_resolve(entity->hold);
    }
}

bool ENT_render_with_flip(
    entity_t *entity
) {
    return entity->direction == LEFT;
}

// returns texture clip from tileset according to current frame and animation
render_img_t ENT_texture_coord(
    entity_t *entity
) {
    if (!(entity->flags & ANIMATIABLE)) {
        return ANIM_texture_coord_full(entity->sheet);
    }
    return ANIM_texture_coord(entity->sheet->animations, entity->state, entity->frame);
}

// calculate where on screen entity texture should be rendered
render_coord_t ENT_img_coord(
    entity_t       *entity,
    render_img_t    texture_coord
) {
    // texture is always rendered 1:1 to achieve pixel perfect effect
    float x1, y1, x2, y2;
    render_coord_t coord;

    float texW = entity->sheet->width;
    float texH = entity->sheet->height;

    // orient it in a way that OpenGL will digest it
    x1 = (float)entity->x - (float)SCREEN_WIDTH / 2.0;
    y1 = (float)entity->y - (float)SCREEN_HEIGHT / 2.0;

    y1 *= -1;
    y2 = (float)y1 - ((texture_coord.y2 - texture_coord.y1) * texH);
    x2 = (float)x1 + ((texture_coord.x2 - texture_coord.x1) * texW);
    y2 = (float)y1 - (float)TILE_WIDTH;
    x2 = (float)x1 + (float)TILE_HEIGHT;

    coord.x1 = x1;
    coord.y1 = y1;
    coord.x2 = x2;
    coord.y2 = y2;

    return coord;
}

void ENT_draw(
    entity_t *entity,
    int x,
    int y
) {
    // do not draw if not needed!
    if (!(entity->flags & NOT_DRAWABLE)) {
        return;
    }

    bool flip                  = ENT_render_with_flip(entity);
    render_img_t texture_coord = ENT_texture_coord(entity);
    render_coord_t img_coord   = ENT_img_coord(entity, texture_coord);

    GFX_render_texture_part(
        sprites[entity->sheet->texture_id],
        img_coord.x1,
        img_coord.y1,
        img_coord.x2,
        img_coord.y2,
        texture_coord.x1, 
        texture_coord.y1, 
        texture_coord.x2, 
        texture_coord.y2, 
        flip
    );

}

void ENT_free(
    entity_t *entity
) {
    if (entity->hold) {
        ENT_free(entity->hold);
        entity->hold = NULL;
    }

    entity->light = NULL;

    if (entity->sheet->n_animations == 0) {
        ANIM_free(entity->sheet);
    }

    entity->sheet = NULL;

    free(entity);
    entity = NULL;
}

int ENT_get_n_hitboxes(
    entity_t *entity
) {
    return entity->sheet->animations[entity->state].frames[entity->frame].n_hit_box;
}

SDL_Rect ENT_get_hitbox(
    entity_t *entity,
    int i
) {
    return entity->sheet->animations[entity->state].frames[entity->frame].hit_boxes[i];
}

lightsource_t* ENT_get_light(
    entity_t *entity
) {
    return entity->light;
}

void ENT_update_collision(
    entity_t *entity,
    segment_t *obstacles
) {
    int  n_boxes     = ENT_get_n_hitboxes(entity);
    SDL_Rect* frame  = NULL;

    int  x_coef      = 0;
    int  y_coef      = 0;
    int  x_collision = -1;
    int  y_collision = -1;
    int  y_falling   = -1;

    y_coef = entity->y_vel;
    x_coef = entity->x_vel;

    sorted_list_t *x_intersections = NULL;
    sorted_list_t *y_intersections = NULL;
    sorted_list_t *falling         = NULL;

    // for each of the hit box x, y and falling collision is checked, and each calculated collision
    // value is stored. Then, depending of the direction of a entity lowest or highest value is then
    // used and final postion of entity.
    for (int i=0; i<n_boxes; i++) {
        SDL_Rect rect = ENT_get_hitbox(entity, i);

        for (segment_t* obstacle=obstacles; obstacle; obstacle=obstacle->next) {
            if (entity->x_vel != 0) {
                x_collision = GEO_vertical_segment_intersects_rect(
                   obstacle->x1,
                   obstacle->y1,
                   obstacle->x2,
                   obstacle->y2, 
                   entity->x + rect.x + x_coef,
                   entity->y + rect.y,
                   entity->x + rect.x + rect.w + x_coef,
                   entity->y + rect.y + rect.h
                );
            }

            y_collision = GEO_horizontal_segment_intersects_rect(
               obstacle->x1,
               obstacle->y1,
               obstacle->x2,
               obstacle->y2, 
               entity->x + rect.x,
               entity->y + rect.y + y_coef,
               entity->x + rect.x + rect.w,
               entity->y + rect.y + rect.h + y_coef
            );

            y_falling = GEO_horizontal_segment_intersects_rect(
               obstacle->x1,
               obstacle->y1,
               obstacle->x2,
               obstacle->y2, 
               entity->x + rect.x,
               entity->y + rect.y + 1,
               entity->x + rect.x + rect.w,
               entity->y + rect.y + rect.h + 1
            );

            if (x_collision != -1) { SRTLST_insert(&x_intersections, x_collision, 0); }
            if (y_collision != -1) { SRTLST_insert(&y_intersections, y_collision, 0); }
            if (y_falling   != -1) { SRTLST_insert(&falling, y_falling, 0 );         }
        }
    }
    
    if (x_intersections != NULL) {
        entity->x_vel = 0;  //stop moving!

        if (entity->direction == LEFT) {
            entity->x = SRTLST_get_last(x_intersections);
        }
        else {
            SDL_Rect *frame = NULL;
            frame = ENT_current_frame_rect(entity);
            entity->x = x_intersections->value - frame->w;
        }
    }

    if (y_intersections != NULL) {

        // moving up
        if (entity->y_vel < 0) { 
            entity->y = SRTLST_get_last(y_intersections);
        }

        // falling down
        else {
            SDL_Rect *frame = ENT_current_frame_rect(entity);
            entity->y = y_intersections->value - frame->h;
            entity->state = STANDING;
        }

        entity->y_vel = 0;  //stop moving!
    }

    if (falling == NULL) {
        // if no obstacles is below entity`s feet failing down begins
        entity->state = FALLING_DOWN;
    }

    free(x_intersections);
    free(y_intersections);
    free(falling);

    if (frame) {
        free(frame);
    }
}

// it is corr
int ENT_light_pt_x(
    entity_t *entity
) {
    SDL_Rect *rect = NULL;
    rect           = ENT_current_frame_rect(entity);
    int x          = 0;

    if (
        entity->light_pt == HANDLE_BACK_UP || 
        entity->light_pt == HANDLE_BACK_MIDDLE || 
        entity->light_pt == HANDLE_BACK_DOWN
    ) {
        if (entity->direction == LEFT) { x = rect->w; }
        else { x = 0; }

    } else if  (
        entity->light_pt == HANDLE_MIDDLE_UP || 
        entity->light_pt == HANDLE_MIDDLE_MIDDLE || 
        entity->light_pt == HANDLE_MIDDLE_DOWN
    ) {
        x = (int) rect->w / 2;
    } else {
        if (entity->direction == RIGHT) { x = rect->w; }
        else { x = 0; }
    }
    free(rect);

    return x;
}

// it is corr
int ENT_light_pt_y(
    entity_t *entity
) {
    SDL_Rect *rect = NULL;
    rect           = ENT_current_frame_rect(entity);
    int y          = 0;

    if (
        entity->light_pt == HANDLE_BACK_UP || 
        entity->light_pt == HANDLE_MIDDLE_UP || 
        entity->light_pt == HANDLE_FRONT_UP 
    ) {
        y = 0;
    } else if  (
        entity->light_pt == HANDLE_BACK_MIDDLE || 
        entity->light_pt == HANDLE_MIDDLE_MIDDLE || 
        entity->light_pt == HANDLE_FRONT_MIDDLE
    ) {
        y = (int) rect->h / 2;
    } else {
        y = rect->h;
    }

    free(rect);

    return y;
}

entity_t* ENT_generate(
    int x,
    int y,
    int id
) {
    return ENT_init(
        x * TILE_WIDTH,
        y * TILE_HEIGHT,
        entity_library[id]
    );
}

// global corrd
int ENT_light_emit_x(
    entity_t *entity
) {
    return entity->x + ENT_light_pt_x(entity);
}

// global corrd
int ENT_light_emit_y(
    entity_t *entity
) {
    return entity->y + ENT_light_pt_y(entity);
}
