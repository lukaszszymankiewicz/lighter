#include <check.h>
#include "../src/files.h"
#include "../src/primitives.h"
#include "../src/import.h"
#include "../src/entity.h"
#include "../src/entity_manager.h"
#include "../src/gfx.h"
#include "../src/segment.h"
#include "../src/sprites.h"


START_TEST (ENTMAN_entity_in_range_check)
{
    // GIVEN 
    int e_x = 5;
    int e_y = 5;
    
    bool res                         = false;
    entity_t *e                      = NULL;
    entity_manager_t* entity_manager = NULL;
    entity_manager                   = ENTMAN_new();

    animations[ASSET_HERO_ANIMATION]         = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    sprites[ASSET_SPRITE_HERO]               = GFX_read_texture(FILEPATH_SPRITE_HERO);
    animations[ASSET_NO_ANIMATION]           = NULL;
    sprites[ASSET_SPRITE_TEST]               = NULL;

    // hero
    ENTMAN_add_entity(entity_manager, e_x, e_y, 0);

    e = NULL;
    e = ENT_generate(24, 24, ENTITY_HERO);
    res = ENTMAN_entity_in_range(entity_manager, e, ENTITY_UPDATE_X_RANGE, ENTITY_UPDATE_Y_RANGE);
    ck_assert_int_eq(res, 0);
}

START_TEST (ENTMAN_apply_collision_check)
{
    // GIVEN 
    int e_x = 5;
    int e_y = 5;

    animations[ASSET_HERO_ANIMATION]         = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    sprites[ASSET_SPRITE_HERO]               = GFX_read_texture(FILEPATH_SPRITE_HERO);

    // hero
    entity_t* e  = NULL;
    e            = ENT_generate(e_x, e_y, ENTITY_HERO);
    e->state     = WALKING;

    // CASE 1
    segment_t* obstacles = NULL;
    SEG_push(&obstacles, 0, e_y * TILE_HEIGHT + 5, 1000, e_y * TILE_HEIGHT + 5);

    ENTMAN_apply_collision(obstacles, e);
    ck_assert_int_ne(e->y, e_y*TILE_HEIGHT);

    // CASE 2
    e            = ENT_generate(e_x, e_y, ENTITY_HERO);
    e->state     = WALKING;
    e->x_vel     = 2;

    obstacles    = NULL;
    SEG_push(&obstacles, e_x * TILE_WIDTH + 2, 0, e_x * TILE_WIDTH + 2, 1000);

    ENTMAN_apply_collision(obstacles, e);
    ck_assert_int_ne(e->x, e_x*TILE_WIDTH);
}

Suite *entity_manager_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("entity_manager");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, ENTMAN_entity_in_range_check);
    tcase_add_test(tc_core, ENTMAN_apply_collision_check);

    suite_add_tcase(s, tc_core);

    return s;
}
