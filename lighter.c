#include "light.h"
#include "events.h"
#include "gfx.h"
#include "sprites.h"
#include "level.h"
#include "config.h"


void close_game() {
    GFX_free();
    TXTR_free_all();
};

void init_game() {
    GFX_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    TXTR_load_frames();
};


int main(int argc, char* args[]) {
    int loop = 1;
    int hero_state = 1;

    int hero_x = 256;
    int hero_y = 256;

    tiles_list_t * tiles = NULL;
    tiles = LVL_read_level();

    SDL_Event event;
    init_game();

    while(loop) {
        EVNT_handle_events(&event, &loop, &hero_state, &(hero_x), &(hero_y));
        GFX_clear_screen();
        TXTR_draw_light(hero_x, hero_y);
        LIG_draw_light_effect(hero_x, hero_y, tiles);
        LVL_draw(tiles);
        TXTR_draw_hero(hero_x-15, hero_y-22, hero_state);
        GFX_update();
    }

    close_game();
	return 0;
}

