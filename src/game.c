#include "global.h"
#include "files.h"
#include "controller.h"
#include "game.h"
#include "gfx.h"
#include "timer.h"
#include "level.h"
#include "light.h"


void GAME_read_all_files() {
    animations[ASSET_HERO_ANIMATION]         = ANIM_read_animation(FILEPATH_HERO_ANIMATION);
    animations[ASSET_NO_ANIMATION]           = NULL;
    gradients[ASSET_GRADIENT_CIRCULAR]       = GFX_read_texture(FILEPATH_GRADIENT_CIRCULAR);
    sprites[ASSET_SPRITE_HERO]               = GFX_read_texture(FILEPATH_SPRITE_HERO);
    sprites[ASSET_SPRITE_TEST]               = GFX_read_texture(FILEPATH_SPRITE_HERO);
    sprites[ASSET_SPRITE_LIGHTER]            = GFX_read_texture(FILEPATH_SPRITE_LIGHTER);
    wobbles[ASSET_WOBBLE_NO]                 = LIG_read_wobble(FILEPATH_WOBBLE_NO);
    wobbles[ASSET_WOBBLE_STABLE]             = LIG_read_wobble(FILEPATH_WOBBLE_STABLE);
    wobbles[ASSET_WOBBLE_WALKING]            = LIG_read_wobble(FILEPATH_WOBBLE_WALKING);
    levels[ASSET_LEVEL_SAMPLE]               = LVL_read_level(FILEPATH_LEVEL_SAMPLE);
    lightsources[ASSET_LIGHTER_LIGHTSOURCE]  = LIG_read_lightsource(FILEPATH_LIGTHER_LIGHTSOURCE);
    lightsources[ASSET_LANTERN_LIGHTSOURCE]  = LIG_read_lightsource(FILEPATH_LANTERN_LIGHTSOURCE);
}

void GAME_update_all_files() {
    lightsources[ASSET_LIGHTER_LIGHTSOURCE]->gradient = gradients[ASSET_GRADIENT_CIRCULAR];
    lightsources[ASSET_LANTERN_LIGHTSOURCE]->gradient = gradients[ASSET_GRADIENT_CIRCULAR];
}

void GAME_free_all_files() {
    ANIM_free(animations[ASSET_HERO_ANIMATION]);
    GFX_free_texture(gradients[ASSET_GRADIENT_CIRCULAR]);
    GFX_free_texture(sprites[ASSET_SPRITE_HERO]);
    LIG_free_wobble(wobbles[ASSET_WOBBLE_NO]);
    LIG_free_wobble(wobbles[ASSET_WOBBLE_STABLE]);
    LIG_free_wobble(wobbles[ASSET_WOBBLE_WALKING]);
    LVL_free(levels[ASSET_LEVEL_SAMPLE]);
    LIG_free_lightsource(lightsources[ASSET_LIGHTER_LIGHTSOURCE]);
    LIG_free_lightsource(lightsources[ASSET_LANTERN_LIGHTSOURCE]);
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
    GAME_free_all_files();
    free(game);
    SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
    GFX_free();
    SDL_Quit();
};

game_t* GAME_init() {
    game_t* game        = NULL;
    game                = (game_t*)malloc(sizeof(game_t));
    keyboard            = CON_init();

    game->loop          = true;
    game->level         = levels[ASSET_LEVEL_SAMPLE];
    game->frame         = 0;
    game->fps_timer     = TIMER_new();
    game->cap_timer     = TIMER_new();

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

void GAME_start_time(
    game_t* game
) {
    TIMER_start(game->fps_timer);
}

void GAME_clear_screen(game_t *game) {
    GFX_clear_screen();
    GFX_clean_buffers();
}

void GAME_update_entities(game_t *game) {
    LVL_update_entities(game->level);
}

void GAME_draw_level(game_t *game) {
    LVL_draw(game->level);
}

void GAME_draw_entities(game_t *game) { 
    LVL_draw_entities(game->level);
}

void GAME_render(game_t *game) {
    GFX_update();
}

void GAME_init_graphics(game_t *game) {
    GFX_init_graphics();
}

int main(
    int argc, char* args[]
) {
    game_t* game = NULL;
    GAME_init_graphics(game);
    GAME_read_all_files();
    GAME_update_all_files();
    game = GAME_init();
    GAME_start_time(game);

    // main game loop
    while(game->loop) {
        GAME_start_cap_time(game);
        GAME_update_events(game);
        GAME_clear_screen(game);
        GAME_update_entities(game);

        // LIG_fill_lightbuffer(
        //     HERO_light_x(game->hero),
        //     HERO_light_y(game->hero),
        //     game->frame,
        //     game->light,
        //     game->level->obstacle_segments,
        //     game->hero->x_vel
        // );
        GAME_draw_level(game);
        // LVL_fill_shadowbuffer_with_tiles(game->level, game->hero->x, game->hero->y, 0);
        // GFX_draw_lightbuffer(
        //     game->light->src->gradient,
        //     game->hero->view_x,
        //     game->hero->view_y
        // );
        GAME_draw_entities(game);

        GAME_render(game);
        GAME_update_ticks(game);
        GAME_update_frame(game);
        GAME_delay_frame(game);
    }

    GAME_close(game);
    
	return 0;
}

