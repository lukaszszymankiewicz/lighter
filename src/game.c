#include "global.h"
#include "game.h"
#include "gfx.h"
#include "sprites.h"
#include "events.h"
#include "hero.h"
#include "timer.h"
#include "tile.h"
#include "level.h"
#include "light.h"
#include "segment.h"

int debug = 2;

void GAME_close(
    game_t *game
) {
   free(game->keys_cooldown);
   TIMER_free(game->cap_timer);
   TIMER_free(game->fps_timer);
   LIG_free(game->light);
   LVL_free(game->level);
   HERO_free(game->hero);
   LIG_free_all_files();
   free(game);
   GFX_free();
   SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
   SDL_Quit();
};

game_t* GAME_init() {
    game_t* game = NULL;
    game         = (game_t*)malloc(sizeof(game_t));

    game->hero          = NULL;
    game->keys_cooldown = NULL;

    game->loop          = true;
    game->hero          = HERO_init(NULL, 12 * TILE_HEIGHT, 12 * TILE_WIDTH); // postion is temp
    game->level         = LVL_read_from_file("./levels/level_1.txt");
    game->light         = LIG_init();
    game->frame         = 0;
    game->fps_timer     = TIMER_new();
    game->cap_timer     = TIMER_new();
    game->keys_state    = SDL_GetKeyboardState(NULL);
    game->keys_cooldown = (Uint8 *)malloc(sizeof(Uint8) * MAX_KEYS);

    return game;
};


void GAME_update(
    game_t* game
) {
    game->frame_ticks = TIMER_get_ticks(game->cap_timer);

    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME) {
        game->frame = (game->frame + 1);
        SDL_Delay(SCREEN_TICKS_PER_FRAME - game->frame_ticks);
    }
}

void GAME_clean_frame(
    game_t* game
) {
    SEG_free(game->level->obstacle_segments);
}

int main(
    int argc, char* args[]
) {
    game_t* game = NULL;
    GFX_init_graphics();
    LIG_read_all_files();
    game = GAME_init();
    EVNT_init();
    TIMER_start(game->fps_timer);

    // main game loop
    while(game->loop) {
        TIMER_start(game->cap_timer);
        EVNT_handle_events(game);
        GFX_clear_screen();
        LVL_analyze(game->level, game->hero->x, game->hero->y);
        HERO_update_friction(game->hero);
        HERO_check_collision(game->hero, game->level->obstacle_segments);
        HERO_update_pos_due_to_velocity(game->hero);
        HERO_update_state(game->hero);
        HERO_update_sprite(game->hero);
        LIG_fill_lightbuffer(
            HERO_light_x(game->hero),
            HERO_light_y(game->hero),
            game->frame,
            game->light,
            game->level->obstacle_segments,
            game->hero->x_vel
        );
        LVL_draw(game->level, game->hero->x, game->hero->y);
        LVL_fill_shadowbuffer_with_tiles(game->level, game->hero->x, game->hero->y, 0);
        GFX_draw_lightbuffer(
            game->light->src->gradient_texture,
            game->hero->view_x,
            game->hero->view_y
        );
        HERO_draw(game->hero);
        GFX_update();
        GAME_update(game);

    }

    GAME_close(game);

	return 0;
}

