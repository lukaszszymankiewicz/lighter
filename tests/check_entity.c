#include <check.h>
#include "../src/assets.h"
#include "../src/segment.h"
#include "../src/files.h"
#include "../src/gfx.h"
#include "../src/entity.h"
#include "../src/controller.h"
#include "../src/sprites.h"

START_TEST (ENT_hold_and_handle_check_empty)
{
    // GIVEN
    sprites[ASSET_SPRITE_LIGHTER] = GFX_read_texture(FILEPATH_SPRITE_LIGHTER);
    entity_t* entity              = NULL;

    entity           = ENT_init(
        0, 100, 100,
        NOTHING_FLAG, HANDLE_BACK_UP, HANDLE_BACK_UP,
        ENTITY_NO, STANDING,
        NULL, sprites[ASSET_SPRITE_LIGHTER], NULL
    );

    int w = sprites[ASSET_SPRITE_LIGHTER]->width;
    int h = sprites[ASSET_SPRITE_LIGHTER]->height;
    int x, y;

    typedef struct testcase {
          int handle;
          int exp_x_l; int exp_y_l;
          int exp_x_r; int exp_y_r;
    } testcase_t;

    testcase_t testcases[] = {
        (testcase_t){ HANDLE_BACK_UP,        w,           0,        0,         0        },
        (testcase_t){ HANDLE_BACK_MIDDLE,    w,           (int)h/2, 0,         (int)h/2 },
        (testcase_t){ HANDLE_BACK_DOWN,      w,           h,        0,         h        },

        (testcase_t){ HANDLE_MIDDLE_UP,      (int)w/2,    0,        (int)w/2,  0        },
        (testcase_t){ HANDLE_MIDDLE_MIDDLE,  (int)w/2,    (int)h/2, (int)w/2,  (int)h/2 },
        (testcase_t){ HANDLE_MIDDLE_DOWN,    (int)w/2,    h,        (int)w/2,  h        },

        (testcase_t){ HANDLE_FRONT_UP,       0,           0,        w,         0        },
        (testcase_t){ HANDLE_FRONT_MIDDLE,   0,           (int)h/2, w,         (int)h/2 },
        (testcase_t){ HANDLE_FRONT_DOWN,     0,           h,        w,         h        },
    };
    int n_cases = 9;

    for (int i=0; i<n_cases; i++) {
        entity->handle    = testcases[i].handle;

        // WHEN
        entity->direction = RIGHT;
        x = ENT_hold_x(entity);
        y = ENT_hold_y(entity);

        // THEN
        ck_assert_int_eq(x, testcases[i].exp_x_r);
        ck_assert_int_eq(y, testcases[i].exp_y_r);

        // WHEN
        entity->direction = LEFT;
        x = ENT_hold_x(entity);
        y = ENT_hold_y(entity);

        // THEN
        ck_assert_int_eq(x, testcases[i].exp_x_l);
        ck_assert_int_eq(y, testcases[i].exp_y_l);
    };

}
END_TEST

START_TEST (ENT_hold_check)
{
    // GIVEN
    sprites[ASSET_SPRITE_HERO]              = GFX_read_texture(FILEPATH_SPRITE_HERO);
    animations[ASSET_HERO_ANIMATION]        = ANIM_read_animation(FILEPATH_HERO_ANIMATION);

    entity_t* entity = NULL;

    int x = 500;
    int y = 400;

    entity = ENT_init(
        0, x, y,
        MOVABLE | STATEABLE | ANIMATIABLE,
        HANDLE_TYPE_NO,
        HANDLE_TYPE_NO,
        ENTITY_NO,
        STANDING,
        animations[ASSET_HERO_ANIMATION],
        sprites[ASSET_SPRITE_HERO],
        NULL
    );

    typedef struct testcase {
          int exp_x_l;
          int exp_y_l;
          int exp_x_r;
          int exp_y_r;
    } testcase_t;

    testcase_t testcases[] = {
        (testcase_t){ 0, 10, 9, 10 },
    };
    int n_cases = 1;

    for (int i=0; i<n_cases; i++) {
        // WHEN
        entity->direction = RIGHT;
        x = ENT_hold_x(entity);
        y = ENT_hold_y(entity);

        // THEN
        ck_assert_int_eq(x, testcases[i].exp_x_r);
        ck_assert_int_eq(y, testcases[i].exp_y_r);

        // WHEN
        entity->direction = LEFT;
        x = ENT_hold_x(entity);
        y = ENT_hold_y(entity);

        // THEN
        ck_assert_int_eq(x, testcases[i].exp_x_l);
        ck_assert_int_eq(y, testcases[i].exp_y_l);
    };
}
END_TEST

START_TEST (ENT_held_item_check)
{
    sprites[ASSET_SPRITE_HERO]              = GFX_read_texture(FILEPATH_SPRITE_HERO);
    sprites[ASSET_SPRITE_LIGHTER]           = GFX_read_texture(FILEPATH_SPRITE_LIGHTER);
    animations[ASSET_HERO_ANIMATION]        = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    lightsources[ASSET_LIGHTSOURCE_LIGHTER] = SRC_read_lightsource(FILEPATH_LIGTHER_LIGHTSOURCE);

    entity_t* entity                  = NULL;
    entity_t* hold                    = NULL;
    
    int x = 500;
    int y = 400;

    entity = ENT_init(
        0, x, y,
        MOVABLE | STATEABLE | ANIMATIABLE,
        HANDLE_TYPE_NO,
        HANDLE_TYPE_NO,
        ENTITY_NO,
        STANDING,
        animations[ASSET_HERO_ANIMATION],
        sprites[ASSET_SPRITE_HERO],
        NULL
    );

    hold           = ENT_init(
        1, 0, 0,
        HOLDABLE,
        HANDLE_MIDDLE_MIDDLE,
        HANDLE_MIDDLE_MIDDLE,
        ENTITY_NO,
        NOTHING,
        NULL,
        sprites[ASSET_SPRITE_LIGHTER],
        NULL
    );

    entity->hold = hold;

    typedef struct testcase {
        int handle;
        int exp_x_r; int exp_y_r; 
        int exp_x_l; int exp_y_l; 
    } testcase_t;

    int w1 = 9;
    int h1 = 20;

    int w2 = 10;
    int h2 = 5;

    testcase_t testcases[] = {
        (testcase_t){HANDLE_BACK_UP,       x+w1,        y+(int)h1/2,           x-w2, y+(int)h1/2           },
        (testcase_t){HANDLE_BACK_MIDDLE,   x+w1,        y+(int)h1/2-(int)h2/2, x-w2, y+(int)h1/2-(int)h2/2 },
        (testcase_t){HANDLE_BACK_DOWN,     x+w1,        y+(int)h1/2-h2,        x-w2, y+(int)h1/2-h2        },

        (testcase_t){HANDLE_MIDDLE_UP,     x+(int)w1/2, y+(int)h1/2,           x-(int)w2/2, y+(int)h1/2           },
        (testcase_t){HANDLE_MIDDLE_MIDDLE, x+(int)w1/2, y+(int)h1/2-(int)h2/2, x-(int)w2/2, y+(int)h1/2-(int)h2/2 },
        (testcase_t){HANDLE_MIDDLE_DOWN,   x+(int)w1/2, y+(int)h1/2-h2,        x-(int)w2/2, y+(int)h1/2-h2        },

        (testcase_t){HANDLE_FRONT_UP,      x+w1-w2,     y+(int)h1/2,           x, y+(int)h1/2           },
        (testcase_t){HANDLE_FRONT_MIDDLE,  x+w1-w2,     y+(int)h1/2-(int)h2/2, x, y+(int)h1/2-(int)h2/2 },
        (testcase_t){HANDLE_FRONT_DOWN,    x+w1-w2,     y+(int)h1/2-h2,        x, y+(int)h1/2-h2        },
    };
    int n_cases = 9;

    for (int i=0; i<n_cases; i++) {
        entity->hold->handle = testcases[i].handle;

        // WHEN
        entity->direction = RIGHT;
        entity->hold->direction = RIGHT;

        x = ENT_held_item_x(entity);
        y = ENT_held_item_y(entity);

        // THEN
        ck_assert_int_eq(x, testcases[i].exp_x_r);
        ck_assert_int_eq(y, testcases[i].exp_y_r);

        // WHEN
        entity->direction = LEFT;
        entity->hold->direction = LEFT;
        x = ENT_held_item_x(entity);
        y = ENT_held_item_y(entity);

        // THEN
        ck_assert_int_eq(x, testcases[i].exp_x_l);
        ck_assert_int_eq(y, testcases[i].exp_y_l);
    };
}
END_TEST

START_TEST (ENT_resolve_check)
{
    // GIVEN
    entity_t* entity         = NULL;

    animations[ASSET_HERO_ANIMATION]   = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    sprites[ASSET_SPRITE_HERO]         = GFX_read_texture(FILEPATH_SPRITE_HERO);
    
    // WHEN
    entity                   = ENT_init(
        0, 100, 100,
        ANIMATIABLE, HANDLE_BACK_UP, HANDLE_BACK_UP, ENTITY_NO, STANDING,
        animations[ASSET_HERO_ANIMATION], sprites[ASSET_SPRITE_HERO], NULL
    );

    entity->sprites->animations[entity->state].frames[0].delay = 2;
    entity->sprites->animations[entity->state].frames[1].delay = 2;
    entity->sprites->animations[entity->state].len = 2;

    // THEN
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
    // hitbox = {0, 0, 9, 20},
    // overall hitbox = { 100, 100, 109, 120 } + 1 (velocity)

    // GIVEN
    animations[ASSET_HERO_ANIMATION]   = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    sprites[ASSET_SPRITE_HERO]         = GFX_read_texture(FILEPATH_SPRITE_HERO);

    entity_t* entity = NULL;
    entity           = ENT_init(
        0, 100, 100,
        APPLY_COLLISION, HANDLE_BACK_UP, HANDLE_BACK_UP, ENTITY_NO, STANDING,
        animations[ASSET_HERO_ANIMATION], sprites[ASSET_SPRITE_HERO], NULL
    );

    segment_t *obstacles = NULL;

    typedef struct testcase {
          int x1; int y1; int x2; int y2;
          int x;  int y;
          int x_vel; int y_vel;
          int direction;
          int exp_x;
    } testcase_t;

    testcase_t testcases[] = {
        (testcase_t){ 105, 80, 105, 130, 100, 100, 1, 0, RIGHT, 78  },
        (testcase_t){ 105, 80, 105, 100, 100, 100, 1, 0, RIGHT, 78  },
        (testcase_t){ 0, 110, 200, 110, 100, 100, 0, 1, RIGHT, 100  },
        (testcase_t){ 105, 80, 105, 130, 100, 100, 1, 0, LEFT, 105  },
    };
    int n_cases = 4;
    
    // WHEN
    for (int i=0; i<n_cases; i++) {
        obstacles = NULL;

        SEG_push(&obstacles,
            testcases[i].x1, testcases[i].y1,
            testcases[i].x2, testcases[i].y2
        );
        entity->x_vel = testcases[i].x_vel;
        entity->y_vel = testcases[i].y_vel;
        entity->x = testcases[i].x;
        entity->y = testcases[i].y;
        entity->direction = testcases[i].direction;

        // THEN
        ENT_update_collision(entity, obstacles);
        ck_assert_int_eq(entity->x, testcases[i].exp_x);
    };
}
END_TEST

START_TEST (ENT_colision_update_check_negatvie)
{
    // GIVEN
    // hitbox = {0, 0, 9, 20},
    // overall hitbox = { 100, 100, 109, 120 } + 1 (velocity)

    animations[ASSET_HERO_ANIMATION]   = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    sprites[ASSET_SPRITE_HERO]         = GFX_read_texture(FILEPATH_SPRITE_HERO);

    entity_t* entity = NULL;
    entity           = ENT_init(
        0, 100, 100,
        APPLY_COLLISION, HANDLE_BACK_UP, HANDLE_BACK_UP,
        ENTITY_NO, STANDING,
        animations[ASSET_HERO_ANIMATION], sprites[ASSET_SPRITE_HERO], NULL
    );

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
        0, 0, 0,
        APPLY_FRICTION, HANDLE_BACK_UP, HANDLE_BACK_UP,
        ENTITY_NO, STANDING,
        NULL, NULL, NULL
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
        0, 0, 0,
        CONTROLABLE | MOVABLE, HANDLE_BACK_UP, HANDLE_BACK_UP,
        ENTITY_NO, STANDING,
        NULL, NULL, NULL
    );

    hold           = ENT_init(
        0, 0, 0,
        HOLDABLE, HANDLE_BACK_UP, HANDLE_BACK_UP,
        ENTITY_NO, STANDING,
        NULL, NULL, NULL
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
    int x          = 420;
    int y          = 69;
    
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

START_TEST (ENT_get_x_and_get_y)
{
    // GIVEN
    sprites[ASSET_SPRITE_HERO]              = GFX_read_texture(FILEPATH_SPRITE_HERO);
    sprites[ASSET_SPRITE_LIGHTER]           = GFX_read_texture(FILEPATH_SPRITE_LIGHTER);
    animations[ASSET_HERO_ANIMATION]        = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    lightsources[ASSET_LIGHTSOURCE_LIGHTER] = SRC_read_lightsource(FILEPATH_LIGTHER_LIGHTSOURCE);

    entity_t* entity                  = NULL;
    entity_t* hold                    = NULL;
    
    int x = 500;
    int y = 400;

    entity = ENT_init(
        0,
        x,
        y,
        MOVABLE | STATEABLE | ANIMATIABLE,
        HANDLE_TYPE_NO,
        HANDLE_TYPE_NO,
        ENTITY_NO,
        STANDING,
        animations[ASSET_HERO_ANIMATION],
        sprites[ASSET_SPRITE_HERO],
        NULL
    );

    hold           = ENT_init(
        1,
        0,
        0,
        HOLDABLE,
        HANDLE_MIDDLE_MIDDLE,
        HANDLE_MIDDLE_MIDDLE,
        ENTITY_NO,
        NOTHING,
        NULL,
        sprites[ASSET_SPRITE_LIGHTER],
        NULL
    );

    entity->hold = hold;

    // WHEN
    ck_assert_int_eq(ENT_get_x(entity), x);
    ck_assert_int_eq(ENT_get_y(entity), y);

    entity->direction = RIGHT;
    ENT_update(entity);

    ck_assert_int_eq(ENT_get_x(entity), x);
    ck_assert_int_eq(ENT_get_y(entity), y);

    entity->direction = LEFT;
    ENT_update(entity);

    ck_assert_int_eq(ENT_get_x(entity), x);
    ck_assert_int_eq(ENT_get_y(entity), y);
}
END_TEST

START_TEST (ENT_light_emit_check)
{
    sprites[ASSET_SPRITE_HERO]              = GFX_read_texture(FILEPATH_SPRITE_HERO);
    sprites[ASSET_SPRITE_LIGHTER]           = GFX_read_texture(FILEPATH_SPRITE_LIGHTER);
    animations[ASSET_HERO_ANIMATION]        = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    lightsources[ASSET_LIGHTSOURCE_LIGHTER] = SRC_read_lightsource(FILEPATH_LIGTHER_LIGHTSOURCE);

    entity_t* entity                  = NULL;
    entity_t* hold                    = NULL;
    
    int x = 500;
    int y = 400;

    entity = ENT_init(
        0, x, y,
        MOVABLE | STATEABLE | ANIMATIABLE,
        HANDLE_TYPE_NO,
        HANDLE_TYPE_NO,
        ENTITY_NO,
        STANDING,
        animations[ASSET_HERO_ANIMATION],
        sprites[ASSET_SPRITE_HERO],
        NULL
    );

    hold           = ENT_init(
        1, 0, 0,
        HOLDABLE,
        HANDLE_MIDDLE_MIDDLE,
        HANDLE_MIDDLE_MIDDLE,
        ENTITY_NO,
        NOTHING,
        NULL,
        sprites[ASSET_SPRITE_LIGHTER],
        NULL
    );

    entity->hold = hold;

    typedef struct testcase {
        int handle;
        int exp_x_r; int exp_y_r; 
        int exp_x_l; int exp_y_l; 
    } testcase_t;

    int w1 = 9;
    int h1 = 20;

    int w2 = 10;
    int h2 = 5;

    testcase_t testcases[] = {
    (testcase_t){HANDLE_BACK_UP,     x+w1,            y+(int)h1/2-(int)h2/2,   x,           y+(int)h1/2-(int)h2/2   },
    (testcase_t){HANDLE_BACK_MIDDLE, x+w1,            y+(int)h1/2,             x,           y+(int)h1/2             },
    (testcase_t){HANDLE_BACK_DOWN,   x+w1,            y+(int)h1/2+(int)h2/2+1, x,           y+(int)h1/2+(int)h2/2+1 },

    (testcase_t){HANDLE_MIDDLE_UP,    x+w1+(int)w2/2, y+(int)h1/2-(int)h2/2,   x-(int)w2/2, y+(int)h1/2-(int)h2/2   },
    (testcase_t){HANDLE_MIDDLE_MIDDLE,x+w1+(int)w2/2, y+(int)h1/2,             x-(int)w2/2, y+(int)h1/2             },
    (testcase_t){HANDLE_MIDDLE_DOWN,  x+w1+(int)w2/2, y+(int)h1/2+(int)h2/2+1, x-(int)w2/2, y+(int)h1/2+(int)h2/2+1 },

    (testcase_t){HANDLE_FRONT_UP,    x+w1+w2,         y+(int)h1/2-(int)h2/2,   x-w2, y+(int)h1/2-(int)h2/2,         },
    (testcase_t){HANDLE_FRONT_MIDDLE,x+w1+w2,         y+(int)h1/2,             x-w2, y+(int)h1/2,                   },
    (testcase_t){HANDLE_FRONT_DOWN,  x+w1+w2,         y+(int)h1/2+(int)h2/2+1, x-w2, y+(int)h1/2+(int)h2/2+1,       },

    };
    int n_cases = 9;

    entity->hold->handle = HANDLE_BACK_MIDDLE;

    for (int i=0; i<n_cases; i++) {
        entity->hold->light_pt = testcases[i].handle;

        // WHEN
        entity->direction = RIGHT;
        entity->hold->direction = RIGHT;
        ENT_update_hold(entity);

        x = ENT_light_emit_x(entity->hold);
        y = ENT_light_emit_y(entity->hold);

        // THEN
        ck_assert_int_eq(x, testcases[i].exp_x_r);
        ck_assert_int_eq(y, testcases[i].exp_y_r);

        // WHEN
        entity->direction = LEFT;
        entity->hold->direction = LEFT;
        ENT_update_hold(entity);
        x = ENT_light_emit_x(entity->hold);
        y = ENT_light_emit_y(entity->hold);

        // THEN
        ck_assert_int_eq(x, testcases[i].exp_x_l);
        ck_assert_int_eq(y, testcases[i].exp_y_l);
    };
}
END_TEST

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
    tcase_add_test(tc_core, ENT_hold_and_handle_check_empty);
    tcase_add_test(tc_core, ENT_hold_check);
    tcase_add_test(tc_core, ENT_get_x_and_get_y);
    tcase_add_test(tc_core, ENT_held_item_check);
    tcase_add_test(tc_core, ENT_light_emit_check);

    suite_add_tcase(s, tc_core);

    return s;
}
