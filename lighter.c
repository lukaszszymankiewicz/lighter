#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "draw.h"
#include "events.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

void close_game() {
    free_graphics();
};

void init_game() {
    init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    load_frames();
};

int main(int argc, char* args[]) {
    int loop = 1;
    int hero_state = 1;

    Polygon poly = {
        {
            {300, 450, 500, 430},
            {500, 430, 500, 500},
            {500, 500, 300, 450},

            {30, 170, 150, 20},
            {150, 20, 200, 200},
            {200, 200, 30, 170},

            {1, 1, 510, 1},
            {510, 1, 510, 510},
            {510, 510, 1, 510},
            {1, 510, 1, 1}
        },
        10,
    };

    Point hero_pos = {
        SCREEN_WIDTH / 2,
        SCREEN_HEIGHT / 2
    };

    SDL_Event event;
    init_game();

    while(loop) {
        handle_events(&event, &loop, &hero_state);
        clear_screen();
        draw_walls(&poly);
        draw_rays(hero_pos.x, hero_pos.y, &poly);
        draw_hero_sprite(hero_pos.x-15, hero_pos.y-22, hero_state);
        update_graphics();
    }

    close_game();
	return 0;
}

