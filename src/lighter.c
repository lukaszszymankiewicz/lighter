#include "config.h"
#include "events.h"
#include "gfx.h"
#include "hero.h"
#include "level.h"
#include "light.h"
#include "timer.h"


void close_game(hero_t * hero_o, light_t * light_o, tiles_list_t * tiles) {
    GFX_free();
    HERO_free(hero_o);
    TILE_free(tiles);
};

void init_game() {
    GFX_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
};


int main(int argc, char* args[]) {
    int loop = 1;
    int counted_frames = 0;

    int frame_ticks;

    SDL_Event event;
    init_game();

    hero_t * hero_o = NULL;
    hero_o = HERO_init();

    tiles_list_t * tiles = NULL;
    tiles = LVL_read_level();

    light_t * light_o = NULL;
    light_o = LIG_init();
    
    game_timer_t* fps_timer = TIMER_new();
    game_timer_t* cap_timer = TIMER_new();

    Texture* bg = TXTR_init_texture("sprites/wall.png");

    TIMER_start(fps_timer);

    while(loop) {
        TIMER_start(cap_timer);
        EVNT_handle_events(&event, &loop, hero_o, light_o);
        GFX_clear_screen();
        // TXTR_render_texture(bg, NULL, 10, 10);  // please be nice, this is temporary!
        LIG_draw_light_effect(
            HERO_light_x(hero_o),
            HERO_light_y(hero_o),
            light_o,
            tiles
        );
        LVL_draw(tiles);
        HERO_draw(hero_o);
        GFX_update();
        ++counted_frames;

        frame_ticks = TIMER_get_ticks(cap_timer);
        if(frame_ticks < SCREEN_TICKS_PER_FRAME )
        {
            SDL_Delay(SCREEN_TICKS_PER_FRAME - frame_ticks);
        }
    }

    close_game(hero_o, light_o, tiles);

	return 0;
}

