#include <SDL2/SDL_timer.h>

#include "data/library.h"

#include "global.h"
#include "controller.h"
#include "entity_manager.h"
#include "game.h"
#include "gfx.h"
#include "timer.h"
#include "source.h"
#include "level.h"
#include "light.h"
#include "animation.h"
#include "texture.h"

#define LEVEL LEVEL_SAMPLE

void GAME_create_library(
) {
    LIB_create_all();
}

void GAME_free_all_files(
) {
    LIB_free_all();
}

void GAME_handle_SDL_events(
    game_t *game
) {
  while (SDL_PollEvent(&(game->event)) != 0) {
    if (game->event.type == SDL_QUIT) { 
        game->loop = false; 
        }
    }
}

void GAME_update_events(
    game_t *game
) {
    GAME_handle_SDL_events(game);
    CON_update(keyboard);
}

void GAME_close(
    game_t *game
) {
    CON_free(keyboard);
    TIMER_free(game->cap_timer);
    TIMER_free(game->fps_timer);
    ENTMAN_free(game->entity_manager);
    LVL_free(game->level);
    GAME_free_all_files();
    free(game);
    GFX_free();
    SDL_Quit();
};

void GAME_init_entities(
    game_t *game
) {
    for (int i=0; i<game->level->n_fill; i++) {
        int x  = game->level->entities_fill[i].x;
        int y  = game->level->entities_fill[i].y;
        int id = game->level->entities_fill[i].id;
        ENTMAN_add_entity(game->entity_manager, x, y, id);
    }
}

game_t* GAME_init(
    int level_id
) {
    game_t* game         = NULL;
    game                 = (game_t*)malloc(sizeof(game_t));

    keyboard             = CON_init();

    game->loop           = true;
    game->entity_manager = ENTMAN_new();

    game->level          = NULL;
    game->level          = levels_library[LEVEL_SAMPLE];

    GAME_init_entities(game);

    game->frame          = 0;
    game->fps_timer      = TIMER_new();
    game->cap_timer      = TIMER_new();

    TIMER_start(game->fps_timer);

    return game;
};

void GAME_update_ticks(
    game_t* game
) {
    game->frame_ticks = TIMER_get_ticks(game->cap_timer);
}

void GAME_update_frame(
    game_t* game
) {
    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME) {
        game->frame = (game->frame + 1);
    }
}

void GAME_delay_frame(
    game_t* game
) {
    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME) {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - game->frame_ticks);
    }
}

void GAME_start_cap_time(
    game_t* game
) {
    TIMER_start(game->cap_timer);
}

void GAME_clear_screen(
    game_t *game
) {
    GFX_clear_screen();
}

void GAME_update_entities(
    game_t *game
) {
    ENTMAT_update(game->entity_manager, game->level->obstacle_segments);
}

void GAME_draw_level(
    game_t *game
) {
    LVL_draw(game->level, ENTMAN_hero_x(game->entity_manager), ENTMAN_hero_y(game->entity_manager));
}

void GAME_draw_entities(
    game_t *game
) { 
    ENTMAN_draw_entities(game->entity_manager);
}

void GAME_render(
    game_t *game
) {
    GFX_update();
}

int GAME_init_graphics(
) {
    return GFX_init_graphics();
}

void GAME_draw_light(
    game_t* game
) {
    light_scene_t* scene = NULL; 
    scene                = LIG_new_light_scene();

    ENTMAN_calc_light(game->entity_manager, scene, game->level->obstacle_segments);

    LIG_compose_light_scene(scene);
    LIG_draw_light_scene(scene);
    LIG_free_light_scene(scene);
}

// TODO: eveything should have success status enabled
game_t* GAME_new(
    int level_id
) {
    game_t* game = NULL;

    int gfx_init_success = 0;
    gfx_init_success = GAME_init_graphics();

    if (!gfx_init_success) { GAME_close(game); }

    GAME_create_library();

    if (LIB_validate() == false ) {
        printf("library not read properly! \n");
        GAME_close(game); 
    }

    game         = GAME_init(level_id);

    return game;
}

void GAME_start_time(
    game_t* game
) {
    GAME_start_cap_time(game);
}

void GAME_apply_logic(
    game_t* game
) {
    GAME_update_events(game);
    GAME_update_entities(game);
}

void GAME_draw(
    game_t* game
) {
    GAME_clear_screen(game);
    GAME_draw_level(game);
    GAME_draw_entities(game);
    GAME_render(game);
    // GAME_draw_light(game);
}

void GAME_update_time(
    game_t* game
) {
    GAME_update_ticks(game);
    GAME_update_frame(game);
    GAME_delay_frame(game);
}

void GAME_loop(
    game_t* game
) {
    while(game->loop) {
        GAME_start_time(game);
        GAME_apply_logic(game);
        GAME_draw(game);
        GAME_update_time(game);
    }
}

void GAME_run(
) {
    game_t* game = NULL;
    game         = GAME_new(LEVEL);
    GAME_loop(game);
    GAME_close(game);
}
