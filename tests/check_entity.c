#include <check.h>
#include "../src/assets.h"
#include "../src/segment.h"
#include "../src/files.h"
#include "../src/import.h"
#include "../src/gfx.h"
#include "../src/entity.h"
#include "../src/controller.h"
#include "../src/sprites.h"

START_TEST (ENT_hold_and_handle_check)
{
    sprites[ASSET_SPRITE_LIGHTER]         = GFX_read_texture(FILEPATH_SPRITE_LIGHTER);
    entity_t* entity = NULL;
    entity           = ENT_init(
        100,
        100,
        NOTHING_FLAG,
        HANDLE_BACK_UP,
        HANDLE_BACK_UP,
        NULL,
        sprites[ASSET_SPRITE_LIGHTER],
        NULL
    );

    int w = sprites[ASSET_SPRITE_LIGHTER]->width;
    int h = sprites[ASSET_SPRITE_LIGHTER]->height;
    int x, y;

    entity->direction = RIGHT;

    // CASE 1
    entity->handle    = HANDLE_BACK_UP;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, 0);
    ck_assert_int_eq(y, 0);

    // CASE 2
    entity->handle    = HANDLE_BACK_MIDDLE;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, 0);
    ck_assert_int_eq(y, (int)h/2);

    // CASE 3
    entity->handle    = HANDLE_BACK_DOWN;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, 0);
    ck_assert_int_eq(y, h);

    // CASE 4
    entity->handle    = HANDLE_MIDDLE_UP;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, (int)w/2);
    ck_assert_int_eq(y, 0);
    
    // CASE 5
    entity->handle    = HANDLE_MIDDLE_MIDDLE;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, (int)w/2);
    ck_assert_int_eq(y, (int)h/2);

    // CASE 6
    entity->handle    = HANDLE_MIDDLE_DOWN;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, (int)w/2);
    ck_assert_int_eq(y, h);

    // CASE 7
    entity->handle    = HANDLE_FRONT_UP;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, w);
    ck_assert_int_eq(y, 0);

    // CASE 8
    entity->handle    = HANDLE_FRONT_MIDDLE;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, w);
    ck_assert_int_eq(y, (int)h/2);

    // CASE 9
    entity->handle    = HANDLE_FRONT_DOWN;
    x = ENT_hold_x(entity);
    y = ENT_hold_y(entity);
    ck_assert_int_eq(x, w);
    ck_assert_int_eq(y, h);

}
END_TEST

START_TEST (ENT_resolve_check)
{
    entity_t* entity         = NULL;
    entity                   = ENT_init(
        100,
        100,
        ANIMATIABLE,
        HANDLE_BACK_UP,
        HANDLE_BACK_UP,
        NULL,
        NULL,
        NULL
    );
    entity->state            = STANDING;

    animations[ASSET_HERO_ANIMATION]   = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    sprites[ASSET_SPRITE_HERO]         = GFX_read_texture(FILEPATH_SPRITE_HERO);

    entity->sprites          = animations[ASSET_HERO_ANIMATION];
    entity->sprites->texture = sprites[ASSET_SPRITE_HERO];

    entity->sprites->animations[entity->state].frames[0].delay = 2;
    entity->sprites->animations[entity->state].frames[1].delay = 2;
    entity->sprites->animations[entity->state].len = 2;

    // WHEN && THEN
    ck_assert_int_eq(entity->frame_t, 0);
    ck_assert_int_eq(entity->frame, 0);

    ENT_resolve(entity);
    ck_assert_int_eq(entity->frame_t, 1);
    ck_assert_int_eq(entity->frame, 0);

    ENT_resolve(entity);
    ck_assert_int_eq(entity->frame_t, 0);
    ck_assert_int_eq(entity->frame, 1);

    ENT_resolve(entity);
    ck_assert_int_eq(entity->frame_t, 1);
    ck_assert_int_eq(entity->frame, 1);

    ENT_resolve(entity);
    ck_assert_int_eq(entity->frame_t, 0);
    ck_assert_int_eq(entity->frame, 0);
}
END_TEST

START_TEST (ENT_colision_update_check_positive)
{
    // GIVEN
    // entity->x = 100;
    // entity->y = 100;
    // hitbox = {0, 0, 9, 20},
    // overall hitbox = { 100, 100, 109, 120 } + 1 (velocity)

    entity_t* entity = NULL;
    entity           = ENT_init(
        100,
        100,
        APPLY_COLLISION,
        HANDLE_BACK_UP,
        HANDLE_BACK_UP,
        NULL,
        NULL,
        NULL
    );

    animations[ASSET_HERO_ANIMATION]   = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    sprites[ASSET_SPRITE_HERO]         = GFX_read_texture(FILEPATH_SPRITE_HERO);
    entity->sprites                    = animations[ASSET_HERO_ANIMATION];
    entity->sprites->texture           = sprites[ASSET_SPRITE_HERO];
    entity->state = STANDING;

    segment_t *obstacles = NULL;

    // CASE 1
    obstacles = NULL;
    SEG_push(&obstacles, 105, 80, 105, 130);
    entity->x = 100;
    entity->y = 100;
    entity->x_vel = 1;
    entity->direction = RIGHT;
    ENT_update_collision(entity, obstacles);
    ck_assert_int_eq(entity->x, 96);

    // CASE 2 (lower edge)
    obstacles = NULL;
    SEG_push(&obstacles, 105, 80, 105, 100);
    entity->x = 100;
    entity->y = 100;
    entity->x_vel = 1;
    ENT_update_collision(entity, obstacles);
    ck_assert_int_eq(entity->x, 96);

    // CASE 3
    obstacles = NULL;
    SEG_push(&obstacles, 0, 110, 200, 110);
    entity->x = 100;
    entity->y = 100;
    entity->y_vel = 1;
    ENT_update_collision(entity, obstacles);
    ck_assert_int_eq(entity->y, 90);

    // CASE 4
    obstacles = NULL;
    SEG_push(&obstacles, 105, 80, 105, 130);
    entity->x = 100;
    entity->y = 100;
    entity->x_vel = 1;
    entity->direction = LEFT;
    ENT_update_collision(entity, obstacles);
    ck_assert_int_eq(entity->x, 105);
}
END_TEST

START_TEST (ENT_colision_update_check_negatvie)
{
    // GIVEN
    // entity as it is have hitbox already in it
    // entity->x = 100;
    // entity->y = 100;
    // hitbox = {0, 0, 9, 20},
    // overall hitbox = { 100, 100, 109, 120 } + 1 (velocity)

    entity_t* entity = NULL;
    entity           = ENT_init(
        100,
        100,
        APPLY_COLLISION,
        HANDLE_BACK_UP,
        HANDLE_BACK_UP,
        NULL,
        NULL,
        NULL
    );

    animations[ASSET_HERO_ANIMATION]   = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    sprites[ASSET_SPRITE_HERO]         = GFX_read_texture(FILEPATH_SPRITE_HERO);
    entity->sprites                    = animations[ASSET_HERO_ANIMATION];
    entity->sprites->texture           = sprites[ASSET_SPRITE_HERO];
    entity->state = STANDING;

    // CASE 1
    segment_t *obstacles = NULL;
    SEG_push(&obstacles, 105, 80, 105, 90);
    entity->state = STANDING;
    entity->x_vel = 1;
    ENT_update_collision(entity, obstacles);
    ck_assert_int_eq(entity->x_vel, 1);

    // CASE 2
    obstacles = NULL;
    SEG_push(&obstacles, 0, 130, 200, 130);  // entity is standing on something (?)
    entity->state = STANDING;
    entity->x = 100;
    entity->y = 100;
    entity->x_vel = 0;
    entity->y_vel = 1;
    ENT_update_collision(entity, obstacles);
    ck_assert_int_eq(entity->y_vel, 1);
    ck_assert_int_eq(entity->y, 100);
    ck_assert_int_eq(entity->x, 100);
}
END_TEST

START_TEST (ENT_with_friction_flag)
{
    // GIVEN
    entity_t* e    = NULL;
    e              = ENT_init(
        0, 
        0,
        APPLY_FRICTION,
        HANDLE_BACK_UP,
        HANDLE_BACK_UP,
        NULL,
        NULL,
        NULL
    );

    // right
    e->direction = RIGHT;
    e->x_vel = 3;
    ENT_update(e);
    ck_assert_int_eq(e->x_vel, 3-X_FRICTION);
    ENT_update(e);
    ck_assert_int_eq(e->x_vel, 3-(2* X_FRICTION));

    e->x_vel = 0;
    ENT_update(e);
    ck_assert_int_eq(e->x_vel, 0);

    // left
    e->direction = LEFT;
    e->x_vel = -3;
    ENT_update(e);
    ck_assert_int_eq(e->x_vel, -3+X_FRICTION);
    ENT_update(e);
    ck_assert_int_eq(e->x_vel, -3+(2* X_FRICTION));

    e->x_vel = 0;
    ENT_update(e);
    ck_assert_int_eq(e->x_vel, 0);
    
    // jumping
    e->state = JUMPING;
    e->x_vel = 0;
    e->y_vel = 10;
    ENT_update(e);
    ck_assert_int_eq(e->y_vel, 10+Y_FRICTION);
    ENT_update(e);
    ck_assert_int_eq(e->y_vel, 10+(2* Y_FRICTION));
}

START_TEST (ENT_with_controllable_flag)
{
    // GIVEN
    entity_t* e    = NULL;
    entity_t* hold = NULL;
    e              = ENT_init(
        0,
        0,
        CONTROLABLE | MOVABLE,
        HANDLE_BACK_UP,
        HANDLE_BACK_UP,
        NULL,
        NULL,
        NULL
    );

    hold           = ENT_init(
        0,
        0,
        HOLDABLE,
        HANDLE_BACK_UP,
        HANDLE_BACK_UP,
        NULL,
        NULL,
        NULL
    );
    keyboard       = CON_init();

    // make entity hold something
    e->hold        = hold;

    // look up
    keyboard->state[SDL_SCANCODE_W] = 1;
    ENT_update(e);
    ck_assert_int_eq(e->hold->direction, UP);

    // look down
    keyboard->state[SDL_SCANCODE_W] = 0;
    keyboard->state[SDL_SCANCODE_S] = 1;
    ENT_update(e);
    ck_assert_int_eq(e->hold->direction, DOWN);

    // reset look
    keyboard->state[SDL_SCANCODE_W] = 0;
    keyboard->state[SDL_SCANCODE_S] = 0;
    ENT_update(e);
    ck_assert_int_eq(e->hold->direction, e->direction);

    // jump
    keyboard->state[SDL_SCANCODE_SPACE] = 1;
    ENT_update(e);
    ck_assert_int_eq(e->state, JUMPING);
    ck_assert_int_ne(e->y_vel, 0);

    // move left
    e->y_vel = 0;
    e->state = NOTHING;
    keyboard->state[SDL_SCANCODE_SPACE] = 0;
    keyboard->state[SDL_SCANCODE_A] = 1;
    ENT_update(e);
    ck_assert_int_eq(e->state, WALKING);
    ck_assert_int_eq(e->direction, LEFT);
    ck_assert_int_ne(e->x_vel, 0);

    // move right
    e->x_vel = 0;
    e->state = NOTHING;
    keyboard->state[SDL_SCANCODE_A] = 0;
    keyboard->state[SDL_SCANCODE_D] = 1;
    ENT_update(e);
    ck_assert_int_eq(e->state, WALKING);
    ck_assert_int_eq(e->direction, RIGHT);
    ck_assert_int_ne(e->x_vel, 0);
}

START_TEST (ENT_generate_check)
{
    entity_t* e    = NULL;
    int x = 13;
    int y = 42;
    
    animations[ASSET_HERO_ANIMATION]         = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    gradients[ASSET_GRADIENT_CIRCULAR]       = GFX_read_texture(FILEPATH_GRADIENT_CIRCULAR);
    sprites[ASSET_SPRITE_HERO]               = GFX_read_texture(FILEPATH_SPRITE_HERO);

    for (int id=0; id<ENTITY_ALL; id++) {
        e = ENT_generate(x, y, id);
        ck_assert_ptr_nonnull(e);
        ck_assert_int_eq(e->x, x*TILE_WIDTH);
        ck_assert_int_eq(e->y, y*TILE_HEIGHT);

        e = NULL;
    }
}

Suite* entity_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("entity");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, ENT_colision_update_check_positive);
    tcase_add_test(tc_core, ENT_colision_update_check_negatvie); 
    tcase_add_test(tc_core, ENT_with_controllable_flag);
    tcase_add_test(tc_core, ENT_with_friction_flag);
    tcase_add_test(tc_core, ENT_resolve_check);
    tcase_add_test(tc_core, ENT_generate_check);
    tcase_add_test(tc_core, ENT_hold_and_handle_check);

    suite_add_tcase(s, tc_core);

    return s;
}
