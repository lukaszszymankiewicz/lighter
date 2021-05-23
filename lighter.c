#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "draw.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512
#define pi 3.14

void init_game();
void close_game();


enum hero_states {
    HERO_STANDING_FRONT = 0,
    HERO_STANDING_LEFT = 1,
    HERO_STANDING_BACK = 2,
    HERO_STANDING_RIGHT = 3,
};

int hero_state = HERO_STANDING_FRONT;

void close_game() {
    free_graphics();
};

void init_game() {
    init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    load_frames();
};

int main(int argc, char* args[]) {
    int loop = 1;
    
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

        // handle events
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                loop = 0;
            }

            else if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                    hero_state = HERO_STANDING_BACK;
                    break;

                    case SDLK_DOWN:
                    hero_state = HERO_STANDING_FRONT;
                    break;

                    case SDLK_RIGHT:
                    hero_state = HERO_STANDING_RIGHT;
                    break;

                    case SDLK_LEFT:
                    hero_state = HERO_STANDING_LEFT;
                    break;
                }
            }

        clear_screen();

        draw_walls(&poly);
        draw_rays(hero_pos.x, hero_pos.y, &poly);
        draw_hero_sprite(hero_pos.x-15, hero_pos.y-22, hero_state);
        update_graphics();

        }
    }

    close_game();
	return 0;
}

