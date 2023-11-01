#include <stdbool.h>

#include "data/library.h"
#include "modules/modules.h"

#include "components.h"
#include "controller.h"
#include "level_manager.h"
#include "entity_manager.h"
#include "game.h"
#include "gfx.h"
#include "global.h"
#include "post.h"
#include "scene.h"
#include "timer.h"

game_t *game      = NULL;

void GAME_handle_window_events(
) {
    while (SDL_PollEvent(&(game->event)) != 0) {
        if (game->event.type == SDL_QUIT) { 
            game->loop = false; 
        }
    }
}

void GAME_init(
    game_config_t config
) {
    game                 = (game_t*)malloc(sizeof(game_t));
    game->loop           = true;
    game->frame          = 0;
    game->config         = config;
};

void GAME_start_time(
) {
    TIMER_start(fps_timer);
    TIMER_start(cap_timer);
}

void GAME_set_camera(
) {
    camera_x = ENTMAN_entity_follow_x(ENTITY_HERO); 
    camera_y = ENTMAN_entity_follow_y(ENTITY_HERO); 
}

void GAME_draw_everything(
) {
    if (!(game->config.use_gfx)) {
        return;
    }

    SCENE_clear();
    GAME_set_camera();

    SCENE_activate_layer(LAYER_LIGHT);
    ENTMAN_calc_light();

    SCENE_activate_layer(LAYER_TILE);
    LVLMAN_draw();

    SCENE_activate_layer(LAYER_SPRITE);
    ENTMAN_draw();

    // SCENE_activate_layer(LAYER_BUFFER);
    // POST_draw();

    SCENE_draw();
    GFX_update();
}

// TODO: assosiate each layer with drawing function
void GAME_fill_scene(
) {
    SCENE_add_layer(LAYER_LIGHT, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    SCENE_add_layer(LAYER_TILE, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    SCENE_add_layer(LAYER_SPRITE, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
    // SCENE_add_buffer_layer(LAYER_BUFFER, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
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
        GAME_handle_window_events();
        CON_update();
        ENTMAT_update();
        GAME_draw_everything(); 
        GAME_update_time();
    }
}

void GAME_new(
    game_config_t config
) {
    GAME_init(config);
    COMPONENTS_init();

    MOD_init(config.use_gfx);
    LIB_init(config.use_gfx);

    GAME_fill_scene();
    LVLMAN_fill(config.level_id);
}

void GAME_close(
) {
    LIB_free_all();
    CON_free();
    TIMER_free(cap_timer);
    TIMER_free(fps_timer);
    ENTMAN_free();
    LVLMAN_free();
    free(game);
    GFX_free();
    SDL_Quit();

    if (game->config.use_gfx) {
        SCENE_free();
    }
};

void GAME_run(
    game_config_t config
) {
    GAME_new(config);
    GAME_loop();
    GAME_close();
}
