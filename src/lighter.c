#include "config.h"
#include "events.h"
#include "gfx.h"
#include "hero.h"
#include "level.h"
#include "light.h"


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

    SDL_Event event;
    init_game();

    hero_t * hero_o = NULL;
    hero_o = HERO_init();

    tiles_list_t * tiles = NULL;
    tiles = LVL_read_level();

    light_t * light_o = NULL;
    light_o = LIG_init();

    while(loop) {
        EVNT_handle_events(&event, &loop, hero_o, light_o);
        GFX_clear_screen();
        LIG_draw_light_effect(hero_o->x, hero_o->y, light_o, tiles);
        LVL_draw(tiles);
        HERO_draw(hero_o);
        GFX_update();
    }

    close_game(hero_o, light_o, tiles);

	return 0;
}

