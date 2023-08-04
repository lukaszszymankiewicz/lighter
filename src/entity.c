#include "data/library.h"

#include "animation.h"
#include "controller.h"
#include "entity.h"
#include "geometry.h"
#include "gl_util.h"
#include "global.h"
#include "scene.h"
#include "sorted_list.h"
#include "source.h"

#define SPRITE_VERTEX_N 16

int state_collisions[3][3] = {
    //  STANDING, WALKING, JUMPING
    {1,            1,         1    },
    {1,            1,         1    },
    {0,            0,         0    },
};

char ENT_flags(
    entity_t *entity
) {
    return entity_library[entity->id]->flags;
}

int ENT_texture_id(
    entity_t *entity
) {
    return entity_library[entity->id]->texture_id;
}

float ENT_light_angle(
    entity_t *entity
) {
    return entity->light.angle;
}

int ENT_lightsource_id(
    entity_t *entity
) {
    return entity_library[entity->id]->lightsource_id;
}

char ENT_light_pt(
    entity_t *entity
) {
    return entity_library[entity->id]->light_emmit_pt;
}

bool ENT_has_flag(
    entity_t *entity,
    int       flag
) {
    return ((entity_library[entity->id]->flags) & flag);
}

bool ENT_has_not_flag(
    entity_t *entity,
    int       flag
) {
    return !(ENT_has_flag(entity, flag));
}

animation_sheet_t ENT_get_animation_sheet(
    entity_t *entity
) {
    return entity_library[entity->id]->animation;
}

lightsource_t* ENT_get_light(
    entity_t *entity
) {
    int id = ENT_lightsource_id(entity);
    return lighsources_library[id];
}

float ENT_wobble_coef(
    entity_t* entity
) {
    wobble_t *wobble = NULL;
    wobble           = ENT_get_wobble(entity);
        
    int frame = entity->anim_frame_t;

    if (wobble->len == 0) {return 0.0;}

    if (frame == 0) {
        return wobble->coefs[0];
    }
    return wobble->coefs[frame%(wobble->len)];
}

int ENT_get_animation_sheet_width(
    entity_t *entity
) {
    return ENT_get_animation_sheet(entity).width;
}

int ENT_get_animation_sheet_height(
    entity_t *entity
) {
    return ENT_get_animation_sheet(entity).height;
}

animation_t ENT_get_animation(
    entity_t *entity,
    int       anim_idx
) {
    return ENT_get_animation_sheet(entity).animations[anim_idx];
}

wobble_t* ENT_get_wobble(
    entity_t *entity
) {
    int id = ENT_get_animation(entity, entity->state).wobble_id;
    return wobble_library[id];
}

frame_t ENT_get_animation_frame(
    entity_t *entity,
    int       anim_idx,
    int       frame_idx
) {
    return ENT_get_animation(entity, anim_idx).frames[frame_idx];
}

frame_t ENT_current_frame(
    entity_t *entity
) {
    return ENT_get_animation_frame(entity, entity->state, entity->anim_frame);
}

int ENT_current_frame_width(
    entity_t *entity
) {
    if (ENT_has_not_flag(entity, ANIMATIABLE)) {
        return ANIM_get_texture_width(ENT_get_animation_sheet(entity));
    }

    return ENT_current_frame(entity).rect.w;
}

int ENT_current_frame_height(
    entity_t *entity
) {
    if (ENT_has_not_flag(entity, ANIMATIABLE)) {
        return ANIM_get_texture_height(ENT_get_animation_sheet(entity));
    }

    return ENT_current_frame(entity).rect.h;
}

int ENT_current_frame_handle_x(
    entity_t *entity
) {
    frame_t f = ENT_current_frame(entity);
    return f.handle_x;
}

int ENT_get_n_hitboxes(
    entity_t *entity
) {
    return ENT_current_frame(entity).n_hit_box;
}

SDL_Rect ENT_get_hitbox(
    entity_t *entity,
    int       hit_box_idx
) {
    return ENT_current_frame(entity).hit_boxes[hit_box_idx];
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

int ENT_handle(
    entity_t *entity
) {
    return entity_library[entity->id]->handle_type;
}

// correction value from handle point to point zero of entity
int ENT_handle_x(
    entity_t *entity
) {
    int w          = ENT_current_frame_width(entity);
    int x          = 0;
    int handle     = ENT_handle(entity);

    if (
        handle == HANDLE_BACK_UP || 
        handle == HANDLE_BACK_MIDDLE || 
        handle == HANDLE_BACK_DOWN
    ) {
        if (entity->direction == LEFT) { x = w; }
        else { x = 0; }

    } else if  (
        handle == HANDLE_MIDDLE_UP || 
        handle == HANDLE_MIDDLE_MIDDLE || 
        handle == HANDLE_MIDDLE_DOWN
    ) {
        x = (int) w / 2;
    } else {
        if (entity->direction == RIGHT) { x = w; }
        else { x = 0; }
    }

    return x;
}

// correction value from handle point to point zero of entity
int ENT_handle_y(
    entity_t *entity
) {
    int h          = ENT_current_frame_height(entity);
    int y          = 0;
    int handle     = ENT_handle(entity);

    if (
        handle == HANDLE_BACK_UP || 
        handle == HANDLE_MIDDLE_UP || 
        handle == HANDLE_FRONT_UP 
    ) {
        y = 0;
    } else if  (
        handle == HANDLE_BACK_MIDDLE || 
        handle == HANDLE_MIDDLE_MIDDLE || 
        handle == HANDLE_FRONT_MIDDLE
    ) {
        y = (int) h / 2;
    } else {
        y = h;
    }

    return y;
}

// where entity has "hand" and holds item, it is corr
int ENT_hold_x(
    entity_t *entity
) {
    if (ENT_has_not_flag(entity, ANIMATIABLE)) {
        return ENT_handle_x(entity);
    }

    frame_t f   = ENT_current_frame(entity);
    int hold_x  = 0;

    if (entity->direction == RIGHT) {
        hold_x = f.handle_x;
    } else {
        hold_x = f.rect.w - f.handle_x;
    }

    return hold_x;
}

// where entity has "hand" and holds item, it is corr
int ENT_hold_y(
    entity_t *entity
) {
    if (ENT_has_not_flag(entity, ANIMATIABLE)) {
        return ENT_handle_y(entity);
    }

    frame_t f   = ENT_current_frame(entity);

    return f.handle_y;
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

void ENT_update_animation(
    entity_t *entity
) {
    entity->anim_frame_t++;

    int del = ENT_current_frame(entity).delay;
    int len = ENT_get_animation(entity, entity->state).len;

    if (entity->anim_frame_t >= del) {
        entity->anim_frame_t = 0;
        entity->anim_frame++; 

        if (entity->anim_frame >= len) {
            entity->anim_frame = 0;
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
        entity->state=FALLING_DOWN; 
    }
}

void ENT_set_light_angle(
    entity_t *entity,
    float     angle
) {
    entity->light.angle = angle;
}

void ENT_direct_holdable_upwards(
    entity_t* entity
) {
    if (!(entity)) {return ;}
    
    if (ENT_has_flag(entity, EMMIT_LIGHT)) {
        float new_angle = SRC_move_lightsource(UP, entity->direction);
        ENT_set_light_angle(entity, new_angle);
    }
}

void ENT_direct_holdable_downwards(
    entity_t* entity
) {
    if (!entity) {return ;}

    if (ENT_has_flag(entity, EMMIT_LIGHT)) {
        float new_angle = SRC_move_lightsource(DOWN, entity->direction);
        ENT_set_light_angle(entity, new_angle);
    }
}

void ENT_direct_holdable_clean(
    entity_t* entity,
    int cur_dir
) {
    if (!entity) { return; }

    if (ENT_has_flag(entity, EMMIT_LIGHT)) {
        float new_angle = SRC_move_lightsource(NONE, cur_dir);
        ENT_set_light_angle(entity, new_angle);
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

    if (ENT_has_flag(entity, EMMIT_LIGHT)) {
        float new_angle = SRC_move_lightsource(NONE, new_direction);
        ENT_set_light_angle(entity, new_angle);
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

    if (ENT_has_flag(entity, EMMIT_LIGHT)) {
        float new_angle = SRC_move_lightsource(NONE, new_direction);
        ENT_set_light_angle(entity, new_angle);
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
    if (ENT_has_not_flag(entity, MOVABLE)) {
        return;
    }
    
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
    if (ENT_has_flag(entity, EMMIT_LIGHT)) {
        return;
    }
    entity->light.frame++;
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
    entity->direction        = LEFT; // this is the default but I donw know why
    entity->x_vel            = 0;
    entity->y_vel            = 0;

    // state
    entity->state            = blueprint->starting_state;
    entity->anim_frame       = 0;
    entity->anim_frame_t     = 0;

    // light
    entity->light            = (entity_light_t) {0, 0, RIGHT_RAD};

    // interactions
    // entity->handle           = blueprint->handle_type;
    // entity->light_pt         = blueprint->light_emmit_pt;
    // entity->flags            = blueprint->flags;
    entity->hold             = NULL;

    // assets
    // entity->sheet            = &blueprint->animation;
    // entity->lightsource_id   = blueprint->lightsource_id;

    // actions
    entity->update_fun_t     = 0;                
    entity->resolution_fun_t = 0;                
    
    int flags = blueprint->flags;

    if (flags & CONTROLABLE) {
        entity->update_fun[entity->update_fun_t++] = ENT_update_control;
    }

    if (flags & APPLY_FRICTION) {
        entity->update_fun[entity->update_fun_t++] = ENT_update_friction;
    }

    if (flags & EMMIT_LIGHT) {
        entity->update_fun[entity->update_fun_t++] = ENT_update_wobble;
    }
    
    // resolutions
    if (flags & MOVABLE) {
        entity->resolution_fun[entity->resolution_fun_t++] = ENT_update_velocity;
    }

    if (flags & STATEABLE) {
        entity->resolution_fun[entity->resolution_fun_t++] = ENT_update_state;
    }

    if (flags & ANIMATIABLE) {
        entity->resolution_fun[entity->resolution_fun_t++] = ENT_update_animation;
    }
    
    if (blueprint->hold_id != ENTITY_NO) {
        entity->resolution_fun[entity->resolution_fun_t++] = ENT_update_hold;
        entity->hold = ENT_generate(x, y, blueprint->hold_id);
        ENT_update_hold(entity);
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
render_coord_t ENT_texture_coord(
    entity_t *entity
) {
    if (ENT_has_not_flag(entity, ANIMATIABLE)) {
        return ANIM_texture_coord_full(ENT_get_animation_sheet(entity));
    }
    return ENT_current_frame(entity).img;
}

render_coord_t ENT_img_coord(
    entity_t       *entity,
    render_coord_t  texture_coord,
    int             camera_x,
    int             camera_y
) {
    float texW = (float) ENT_get_animation_sheet_width(entity);
    float texH = (float) ENT_get_animation_sheet_height(entity);

    render_coord_t render = GL_UTIL_global_to_gl_coord(
        entity->x, entity->y,
        texW, texH,
        texture_coord.x1, texture_coord.y1,
        texture_coord.x2, texture_coord.y2
    );

    render = GL_UTIL_gl_to_camera_gl(render, camera_x, camera_y);
 
    return render;
}

void ENT_add_to_scene(
    entity_t *entity,
    int camera_x, int camera_y
) {
    if (ENT_has_flag(entity, NOT_DRAWABLE)) {
        return;
    }

    bool           flip       = ENT_render_with_flip(entity);
    int            texture_id = ENT_texture_id(entity);
    render_coord_t clip       = ENT_texture_coord(entity);
    render_coord_t render     = ENT_img_coord(entity, clip, camera_x, camera_y);
    
    // TODO: uglyy (power up the render_coord_t)!!
    float vertices[] = {
    //  Position      Texcoords
        render.x1, render.y1, 0.0f, 0.0f, // Top-left
        render.x2, render.y1, 1.0f, 0.0f, // Top-right
        render.x2, render.y2, 1.0f, 1.0f, // Bottom-right
        render.x1, render.y1, 0.0f, 1.0f  // Bottom-left
    };

    SCENE_add_shader(
        scene,
        LAYER_SPRITE,
        texture_id,
        SHADER_TEXTURE,
        SPRITE_VERTEX_N ,
        vertices,
        NULL
    );
}

void ENT_free(
    entity_t *entity
) {
    if (entity->hold) {
        ENT_free(entity->hold);
        entity->hold = NULL;
    }

    free(entity);
    entity = NULL;
}

void ENT_update_collision(
    entity_t  *entity,
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
            int w = ENT_current_frame_width(entity);
            entity->x = x_intersections->value - w;
        }
    }

    if (y_intersections != NULL) {

        // moving up
        if (entity->y_vel < 0) { 
            entity->y = SRTLST_get_last(y_intersections);
        }

        // falling down
        else {
            int h = ENT_current_frame_height(entity);
            entity->y = y_intersections->value - h;
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
    int w        = ENT_current_frame_width(entity);
    int x        = 0;
    int light_pt = ENT_light_pt(entity);

    if (
        light_pt == HANDLE_BACK_UP || 
        light_pt == HANDLE_BACK_MIDDLE || 
        light_pt == HANDLE_BACK_DOWN
    ) {
        if (entity->direction == LEFT) { x = w; }
        else { x = 0; }

    } else if  (
        light_pt == HANDLE_MIDDLE_UP || 
        light_pt == HANDLE_MIDDLE_MIDDLE || 
        light_pt == HANDLE_MIDDLE_DOWN
    ) {
        x = (int) w / 2;
    } else {
        if (entity->direction == RIGHT) { x = w; }
        else { x = 0; }
    }

    return x;
}

// it is corr
int ENT_light_pt_y(
    entity_t *entity
) {
    int h        = ENT_current_frame_height(entity);
    int y        = 0;
    int light_pt = ENT_light_pt(entity);

    if (
        light_pt == HANDLE_BACK_UP || 
        light_pt == HANDLE_MIDDLE_UP || 
        light_pt == HANDLE_FRONT_UP 
    ) {
        y = 0;
    } else if  (
        light_pt == HANDLE_BACK_MIDDLE || 
        light_pt == HANDLE_MIDDLE_MIDDLE || 
        light_pt == HANDLE_FRONT_MIDDLE
    ) {
        y = (int) h / 2;
    } else {
        y = h;
    }

    return y;
}

entity_t* ENT_generate(
    int x,
    int y,
    int id
) {
    return ENT_init(x, y, entity_library[id]);
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

