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
    Position hero_pos = {256, 100};

    // yeah, it should be taken from some file or something but let it be like this by now
    PolyLine * walls = NULL;
    insert_PolyLine(&walls, new_PolyLine(300, 450, 500, 430));
    insert_PolyLine(&walls, new_PolyLine(500, 430, 500, 500));
    insert_PolyLine(&walls, new_PolyLine(500, 500, 300, 450));
    insert_PolyLine(&walls, new_PolyLine(30, 170, 150, 20));
    insert_PolyLine(&walls, new_PolyLine(150, 20, 200, 200));
    insert_PolyLine(&walls, new_PolyLine(200, 200, 30, 170));
    insert_PolyLine(&walls, new_PolyLine(500, 20, 300, 130));
    insert_PolyLine(&walls, new_PolyLine(300, 130, 400, 230));
    insert_PolyLine(&walls, new_PolyLine(400, 230, 500, 20));
    insert_PolyLine(&walls, new_PolyLine(1, 1, 510, 1));
    insert_PolyLine(&walls, new_PolyLine(510, 1, 510, 510));
    insert_PolyLine(&walls, new_PolyLine(510, 510, 1, 510));
    insert_PolyLine(&walls, new_PolyLine(1, 510, 1, 1));

    SDL_Event event;
    init_game();

    while(loop) {
        handle_events(&event, &loop, &hero_state, &(hero_pos.x), &(hero_pos.y));
        LRE_clear_screen();
        LRE_draw_rays(hero_pos, walls);

        // such walls is only for presentation purposes
        LRE_fill_triangle(300, 450, 500, 430, 500, 500, 28, 73, 102, 255);
        LRE_fill_triangle(30, 170, 150, 20, 200, 200, 28, 73, 102, 255);
        LRE_fill_triangle(500, 20, 300, 130, 400, 230, 28, 73, 102, 255);
        LRE_draw_hero_sprite(hero_pos.x-15, hero_pos.y-22, hero_state);
        LRE_update_graphics();
    }

    close_game();
	return 0;
}

