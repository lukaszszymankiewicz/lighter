#include <stdbool.h>
#include <stdio.h>

#include "data/library.h"

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

void GAME_create_library(
) {
    LIB_create_all();
}

void GAME_read_modules(
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
    CON_update(keyboard);
}

void GAME_close(
    game_t *game
) {
    GAME_free_all_files();
    CON_free(keyboard);
    TIMER_free(game->cap_timer);
    TIMER_free(game->fps_timer);
    ENTMAN_free(game->entity_manager);
    LVL_free(game->level);
    free(game);
    GFX_free();
    SDL_Quit();
    SCENE_free(scene);
};

void GAME_init_entities(
    game_t *game
) {
    int n_fills = LVL_n_entity_fills(game->level);

    for (int i=0; i<n_fills; i++) {
        int x  = levels_library[game->level->blueprint_id]->entities[i].x;
        int y  = levels_library[game->level->blueprint_id]->entities[i].y;
        int id = levels_library[game->level->blueprint_id]->entities[i].id;

        ENTMAN_add_entity(game->entity_manager, x, y, id);
    }
}

game_t* GAME_init(
    int level_id,
    int max_frames
) {
    game_t* game         = NULL;
    game                 = (game_t*)malloc(sizeof(game_t));

    game->max_frames     = max_frames;

    keyboard             = CON_init();

    game->loop           = true;
    game->entity_manager = ENTMAN_new();
    
    game->level          = NULL;
    game->level          = LVL_new(level_id);

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

void GAME_start_cap_time(
    game_t* game
) {
    TIMER_start(game->cap_timer);
}

void GAME_update_entities(
    game_t *game
) {
    ENTMAT_update(game->entity_manager, game->level->obstacle_segments);
}

void GAME_draw_light(
    game_t* game
) {
    ENTMAN_calc_light(
        game->entity_manager,
        game->level->obstacle_segments
    );
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

void GAME_set_camera(
    game_t* game
) {
    icamera_x = ENTMAN_hero_x(game->entity_manager);
    icamera_y = ENTMAN_hero_y(game->entity_manager);
    camera_x = GL_UTIL_x(icamera_x);
    camera_y = GL_UTIL_y(icamera_y);
}

void GAME_draw_everything(
    game_t* game
) {
    SCENE_clear();
    GAME_set_camera(game);
    LVL_draw(game->level);

    // TODO: put_on_scene -> draw
    ENTMAN_put_on_scene(game->entity_manager);

    GAME_draw_light(game);
    POST_draw();
    SCENE_draw(scene);
    GFX_update();
}

void GAME_draw_nothing(
    game_t* game
) { }

void GAME_init_scene(
) {
    SCENE_new();
    SCENE_add_layer(LAYER_TILE, SHADER_TEXTURE, GL_TRIANGLES);
    SCENE_add_layer(LAYER_SPRITE, SHADER_TEXTURE, GL_POLYGON);
    SCENE_add_layer(LAYER_LIGHT, SHADER_LIGHT, GL_POLYGON);
    SCENE_add_buffer_layer(LAYER_BUFFER, SHADER_TEXTURE, GL_TRIANGLES);
}

void GAME_init_level(
    game_t *game
) {
    LVL_fill(game->level);
    LVL_analyze(game->level);
}

game_t* GAME_new(
    int level_id,
    int graphic_option,
    int max_frames
) {
    game_t *game      = NULL;
    
    // TODO: read-> init, create ->init
    GAME_read_modules();
    GAME_create_library();

    game              = GAME_init(level_id, max_frames);
    
    // TODO: config should do this, also be not afraid of one more `if` for each frame
    if(graphic_option == GRAPHIC_ON) {
        game->draw_func = &GAME_draw_everything; 
    } else {
        game->draw_func = &GAME_draw_nothing; 
    }

    GAME_init_scene();
    GAME_init_entities(game);
    GAME_init_level(game);

    return game;
}

void GAME_draw(
    game_t* game
) {
    game->draw_func(game);
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
    if (!game) {
        return;
    }

    while((game->loop) && (game->frame != game->max_frames)) {
        GAME_start_time(game);
        GAME_apply_logic(game);
        GAME_draw(game);
        GAME_update_time(game);
        // game->loop = false;
    }

    return;
}

void GAME_run(
    int level_id,
    int graphic_option,
    int max_frames
) {
    game_t* game = NULL;
    game         = GAME_new(level_id, graphic_option, max_frames);
    GAME_loop(game);
    GAME_close(game);
}
