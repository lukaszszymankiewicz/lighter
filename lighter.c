#include "config.h"
#include "events.h"
#include "gfx.h"
#include "hero.h"
#include "level.h"
#include "light.h"
#include "sprites.h"


void close_game(hero_t * hero_o) {
    GFX_free();
    TXTR_free_all();
    HERO_free(hero_o);
};

void init_game() {
    GFX_init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    TXTR_load_frames();
};


int main(int argc, char* args[]) {
    int loop = 1;

    SDL_Event event;
    init_game();

    hero_t * our_hero = NULL;
    our_hero = HERO_init();

    tiles_list_t * tiles = NULL;
    tiles = LVL_read_level();

    while(loop) {
        EVNT_handle_events(&event, &loop, our_hero);
        GFX_clear_screen();
        LIG_draw_light_effect(our_hero->x, our_hero->y, tiles, our_hero->light_source);
        LVL_draw(tiles);
        HERO_draw(our_hero);
        GFX_update();

    }

    close_game(our_hero);
	return 0;
}

