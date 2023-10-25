#include <stdbool.h>
#include <stdio.h>

#include "data/library.h"
#include "modules/modules.h"

#include "components.h"
#include "controller.h"
#include "level_manager.h"
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

// TODO: make event_manager
void GAME_handle_SDL_events(
) {
    while (SDL_PollEvent(&(game->event)) != 0) {
        if (game->event.type == SDL_QUIT) { 
            game->loop = false; 
        }
    }
}

void GAME_update_events(
) {
    GAME_handle_SDL_events();
    CON_update();
}

void GAME_close(
) {
    LIB_free_all();
    CON_free();
    TIMER_free(cap_timer);
    TIMER_free(fps_timer);
    ENTMAN_free();
    LVL_free(level_manager->level);
    free(game);
    GFX_free();
    SDL_Quit();

    if (game->config.use_gfx) {
        SCENE_free();
    }
};

void GAME_init(
) {
    game                 = (game_t*)malloc(sizeof(game_t));
    game->loop           = true;
    game->frame          = 0;
};

void GAME_update_entities(
) {
    ENTMAT_update(level_manager->level->obstacle_segments);
}

void GAME_start_time(
) {
    TIMER_start(fps_timer);
    TIMER_start(cap_timer);
}

void GAME_apply_logic(
) {
    GAME_update_events();
    GAME_update_entities();
}

void GAME_set_camera(
) {
    // TODO: make it better
    camera_x = ENTMAN_hero_x() - SCREEN_WIDTH / 2;
    camera_y = ENTMAN_hero_y() - SCREEN_HEIGHT / 2;
}

void GAME_draw_everything(
) {
    if (!(game->config.use_gfx)) {
        return;
    }

    SCENE_clear();
    GAME_set_camera();

    SCENE_activate_layer(LAYER_TILE);
    LVL_draw(level_manager->level);

    SCENE_activate_layer(LAYER_SPRITE);
    ENTMAN_draw();

    SCENE_activate_layer(LAYER_LIGHT);
    ENTMAN_calc_light(level_manager->level->obstacle_segments);

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

void GAME_new(
    game_config_t config
) {
    GAME_init();
    COMPONENTS_init();

    MOD_init(config.use_gfx);
    LIB_init(config.use_gfx);

    GAME_fill_scene();
    LVLMAN_fill(config.level_id);
}

void GAME_update_time(
) {
    // update ticks
    game->frame_ticks = TIMER_get_ticks(cap_timer);
    
    // update frame
    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME) {
        game->frame++;
    }
    // delay frame if needed
    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME) {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - game->frame_ticks);
    }
}

bool GAME_shold_run(
) {
    return (game->loop) && (game->frame != game->config.max_frames);
}

void GAME_loop(
) {
    if (!game) {
        return;
    }

    while(GAME_shold_run() ) {
        GAME_start_time();
        GAME_apply_logic();
        GAME_draw_everything(); 
        GAME_update_time();
        // game->loop = false;
    }
}

void GAME_run(
    game_config_t config
) {
    GAME_new(config);
    GAME_loop();
    GAME_close();
}
