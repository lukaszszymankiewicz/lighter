#include <SDL2/SDL_timer.h>
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
#include "global.h"
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
) {
    game                 = (game_t*)malloc(sizeof(game_t));
    game->loop           = true;
    game->frame          = 0;
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
    printf("\n\nFRAME %d ---------- \n", game->frame);

    SCENE_clear();
    GAME_set_camera();
    
    SCENE_activate_buffer(FIRST_BUFFER);
        SCENE_activate_layer(LAYER_TILE);
            LVLMAN_put_to_scene();
            SCENE_render_current_layer();

         SCENE_activate_layer(LAYER_SPRITE);
             ENTMAN_put_to_scene();
             SCENE_render_current_layer();

    SCENE_activate_buffer(FIRST_BUFFER);
        SCENE_activate_layer(SCALED_IMAGE);
            SCENE_draw_scaled_buffer();

    SCENE_activate_buffer(DEFAULT_FRAMEBUFFER);
        SCENE_activate_layer(SCALED_IMAGE);
            SCENE_render_current_layer();
    
    // cleanup
    SCENE_clean_buffer(FIRST_BUFFER);
    SCENE_clear_layer(SCALED_IMAGE);
    GAME_set_camera();

    // light
    SCENE_activate_buffer(FIRST_BUFFER);
        SCENE_activate_layer(LAYER_LIGHT);
            ENTMAN_put_light_to_scene();
            SCENE_render_current_layer();

        SCENE_activate_layer(SCALED_IMAGE);
            SCENE_draw_scaled_buffer();

    SCENE_activate_buffer(DEFAULT_FRAMEBUFFER);
        SCENE_activate_layer(SCALED_IMAGE);
            SCENE_render_current_layer();

    GFX_update();
}

void GAME_fill_scene(
) {
    SCENE_add_defalt_buffer();

    SCENE_add_layer(LAYER_LIGHT, "Light");
    SCENE_add_layer(LAYER_TILE, "Tiles");
    SCENE_add_layer(LAYER_SPRITE, "Sprites");
    SCENE_add_layer(SCALED_IMAGE, "Pixelated Image");
    
    SCENE_add_buffer(FIRST_BUFFER, SCREEN_WIDTH, SCREEN_HEIGHT);
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
    return (game->loop) && (game->frame != -1);
}

void GAME_loop(
) {
    if (!game) {
        return;
    }

    while(GAME_shold_run()) {

        GAME_start_time();
        GAME_handle_window_events();
        CON_update();

        if (CON_button_is_just_released(SDL_SCANCODE_Q)) {
            game->loop = false;
        }
        ENTMAT_update();
        GAME_draw_everything(); 
        GAME_update_time();
    }
}

void GAME_new(
) {
    GAME_init();
    COMPONENTS_init();

    MOD_init();
    LIB_init();

    GAME_fill_scene();
    LVLMAN_fill(0);
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
    SCENE_free();
};

void GAME_run(
) {
    GAME_new();
    GAME_loop();
    GAME_close();
}
