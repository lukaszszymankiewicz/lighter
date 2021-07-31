#include "def.h"
#include "game.h"
#include "gfx.h"
#include "sprites.h"
#include "events.h"
#include "hero.h"
#include "timer.h"
#include "tile.h"
#include "level.h"
#include "light.h"


void GAME_close(game_t* game) 
{
 GFX_free();
 HERO_free(game->hero);
 TILE_free(game->tiles);
 LIG_free(game->light);
 TIMER_free(game->cap_timer);
 TIMER_free(game->fps_timer);
};

game_t * GAME_init() 
{
    game_t* game = (game_t*)malloc(sizeof(game_t));

    game->loop = true;
    game->hero = NULL;
    game->hero = HERO_init(NULL);

    game->tiles = LVL_read_level();
    game->light = LIG_init();

    game->frame = 0;
    game->fps_timer = TIMER_new();
    game->cap_timer = TIMER_new();

    game->keys_state = SDL_GetKeyboardState(NULL);
    game->keys_cooldown = NULL;

    int n_keyboard_keys;
    SDL_GetKeyboardState(&n_keyboard_keys);
    game->keys_cooldown = (Uint8 *)malloc(sizeof(Uint8) * n_keyboard_keys);

    return game;
};


void GAME_update(game_t* game)
{
    game->frame_ticks = TIMER_get_ticks(game->cap_timer);

    if(game->frame_ticks < SCREEN_TICKS_PER_FRAME)
    {
        game->frame = (game->frame + 1);
        SDL_Delay(SCREEN_TICKS_PER_FRAME - game->frame_ticks);
    }
}

int main(int argc, char* args[]) {
    // TODO: this can be surely in game struct?
    GFX_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    game_t* game = GAME_init();
    EVNT_init();

    Texture* bg = TXTR_init_texture("sprites/wall.png");

    TIMER_start(game->fps_timer);

    while(game->loop) 
    {
        TIMER_start(game->cap_timer);

        EVNT_handle_events(game);

        GFX_clear_screen();
        TXTR_render_texture(bg, NULL, 10, 10, 0);
        LIG_draw_light_effect(
            HERO_light_x(game->hero), HERO_light_y(game->hero), game->light, game->tiles, game->frame
        );
        LVL_draw(game->tiles);
        HERO_draw(game->hero);
        HERO_update(game->hero);
        GFX_update();
        GAME_update(game);
    }

    GAME_close(game);

	return 0;
}

