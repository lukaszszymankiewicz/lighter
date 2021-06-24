#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "draw.h"
#include "events.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

void close_game() {
    LRE_free_graphics();
};

void init_game() {
    init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    load_frames();
};

int main(int argc, char* args[]) {
    int loop = 1;
    int hero_state = 1;
    position_t hero_pos = {256, 256};

    // yeah, it should be taken from some file or something but let it be like this by now
    polyline_t * walls = NULL;

    insert_polyline(&walls, init_polyline(300, 450, 500, 430));
    insert_polyline(&walls, init_polyline(500, 430, 500, 500));
    insert_polyline(&walls, init_polyline(500, 500, 300, 450));

    insert_polyline(&walls, init_polyline(30, 170, 150, 20));
    insert_polyline(&walls, init_polyline(150, 20, 200, 200));
    insert_polyline(&walls, init_polyline(200, 200, 30, 170));

    insert_polyline(&walls, init_polyline(500, 20, 300, 130));
    insert_polyline(&walls, init_polyline(300, 130, 400, 230));
    insert_polyline(&walls, init_polyline(400, 230, 500, 20));

    insert_polyline(&walls, init_polyline(1, 1, 511, 1));
    insert_polyline(&walls, init_polyline(511, 1, 511, 511));
    insert_polyline(&walls, init_polyline(511, 511, 1, 511));
    insert_polyline(&walls, init_polyline(1, 511, 1, 1));

    SDL_Event event;
    init_game();

    while(loop) {
        handle_events(&event, &loop, &hero_state, &(hero_pos.x), &(hero_pos.y));
        LRE_clear_screen();

        LRE_draw_ligh_gradient(hero_pos.x-350, hero_pos.y-350);
        LRE_draw_lightpoly(hero_pos, walls);

        LRE_fill_triangle(300, 450, 500, 430, 500, 500, 30, 73, 102, 255);
        LRE_fill_triangle(30, 170, 150, 20, 200, 200, 30, 73, 102, 255);
        LRE_fill_triangle(500, 20, 300, 130, 400, 230, 30, 73, 102, 255);

        LRE_draw_hero_sprite(hero_pos.x-15, hero_pos.y-22, hero_state);
        LRE_update_graphics();
    }

    close_game();
	return 0;
}

