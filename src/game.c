#include <stdbool.h>
#include <stdio.h>

#include "data/library.h"

#include "components.h"
#include "controller.h"
#include "entity_manager.h"
#include "game.h"
#include "gfx.h"
#include "gl_util.h"
#include "global.h"
#include "level.h"
#include "light.h"
#include "point.h"
#include "post.h"
#include "render.h"
#include "scene.h"
#include "timer.h"

game_t *game      = NULL;

void GAME_init_library(
) {
    LIB_create_all();
}

void GAME_init_modules(
) {
    LIB_init_all_modules();
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
    CON_update();
}

void GAME_close(
    game_t *game
) {
    GAME_free_all_files();
    CON_free();
    TIMER_free(game->cap_timer);
    TIMER_free(game->fps_timer);
    ENTMAN_free();
    LVL_free(game->level);
    free(game);
    GFX_free();
    SDL_Quit();
    SCENE_free();
};

void GAME_fill_entities(
) {
    int n_fills = LVL_n_entity_fills(game->level);

    for (int i=0; i<n_fills; i++) {
        int blueprint_id = game->level->blueprint_id;
        int x            = levels_library[blueprint_id]->entities[i].x;
        int y            = levels_library[blueprint_id]->entities[i].y;
        int id           = levels_library[blueprint_id]->entities[i].id;

        ENTMAN_add_entity(x, y, id);
    }
}

void GAME_init_global_components(
) {
    CON_init();
    ENTMAN_init();
    TIMER_init_fps_timer();
    TIMER_init_cap_timer();
    SCENE_init();
}

void GAME_init(
    game_config_t config
) {
    game                 = (game_t*)malloc(sizeof(game_t));

    game->loop           = true;
    game->frame          = 0;

    game->level          = NULL;
    game->level          = LVL_new(game->config.level_id);

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
        game->frame++;
    }
}

void GAME_delay_frame(
    game_t* game
) {
    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME) {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - game->frame_ticks);
    }
}

void GAME_update_entities(
    game_t *game
) {
    ENTMAT_update(game->level->obstacle_segments);
}

void GAME_draw_light(
) {
    ENTMAN_calc_light(game->level->obstacle_segments);
}

void GAME_start_time(
    game_t* game
) {
    TIMER_start(fps_timer);
    TIMER_start(cap_timer);
}

void GAME_apply_logic(
    game_t* game
) {
    GAME_update_events(game);
    GAME_update_entities(game);
}

void GAME_set_camera(
) {
    // TODO: make it better
    camera_x = ENTMAN_hero_x() - SCREEN_WIDTH / 2;
    camera_y = ENTMAN_hero_y() - SCREEN_HEIGHT / 2;
}

void GAME_draw_everything(
) {
    SCENE_clear();
    GAME_set_camera();

    SCENE_activate_layer(LAYER_TILE);
    LVL_draw(game->level);

    SCENE_activate_layer(LAYER_SPRITE);
    ENTMAN_draw();

    SCENE_activate_layer(LAYER_LIGHT);
    GAME_draw_light();

    // POST_draw();
    SCENE_draw();
    GFX_update();
}

void GAME_fill_scene(
) {
    POST_set();

    SCENE_add_layer(LAYER_TILE, SHADER_TEXTURE, GL_TRIANGLES);
    SCENE_add_layer(LAYER_SPRITE, SHADER_TEXTURE, GL_POLYGON);
    SCENE_add_layer(LAYER_LIGHT, SHADER_LIGHT, GL_POLYGON);
    // SCENE_add_buffer_layer(LAYER_BUFFER, SHADER_TEXTURE, GL_TRIANGLES);
}

void GAME_fill_level(
) {
    LVL_fill(game->level);
    LVL_analyze(game->level);
}

game_t* GAME_new(
    game_config_t config
) {
    GAME_init(config);
    GAME_init_global_components();
    GAME_init_modules();
    GAME_init_library();

    GAME_fill_scene();
    GAME_fill_entities();
    GAME_fill_level();

    return game;
}

void GAME_draw(
    game_t* game
) {
    GAME_draw_everything(); 
}

void GAME_update_time(
    game_t* game
) {
    GAME_update_ticks(game);
    GAME_update_frame(game);
    GAME_delay_frame(game);
}

bool GAME_shold_run(
) {
    return (game->loop) && (game->frame != game->config.max_frames);
}

void GAME_loop(
    game_t* game
) {
    if (!game) {
        return;
    }

    while( GAME_shold_run() ) {
        GAME_start_time(game);
        GAME_apply_logic(game);
        GAME_draw(game);
        GAME_update_time(game);
        // game->loop = false;
    }
}

void GAME_run(
    game_config_t config
) {
    game         = GAME_new(config);
    GAME_loop(game);
    GAME_close(game);
}
