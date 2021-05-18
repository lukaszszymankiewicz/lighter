#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include "draw.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

void init_game();
void close_game();

int hero_state = 1;
int radius = 128;
int s_radius_ratio = 2;
float half_pi = 1.57;

enum hero_states {
    HERO_STANDING_FRONT = 0,
    HERO_STANDING_LEFT = 1,
    HERO_STANDING_BACK = 2,
    HERO_STANDING_RIGHT = 3,
};

void close_game() {
    free_graphics();
};

void init_game() {
    init_graphics(SCREEN_WIDTH, SCREEN_HEIGHT);
    load_frames();
};

int main(int argc, char* args[]) {
    int loop = 1;

    int x = SCREEN_HEIGHT / 2;
    int y = SCREEN_WIDTH / 2;
    int resolution = 128;

    // because we calculate hero rays for quater of full circle (other are just symetries)
    float deg_res = half_pi / 2 / resolution; 

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

        // render light
        for (float deg=0; deg<half_pi; deg+=deg_res) {
            int ray_end_x = floor(x + cos(deg) * radius);
            int ray_end_y = floor(y + sin(deg) * radius);

            int ray_beg_x = floor(x + cos(deg) * (radius >> s_radius_ratio));
            int ray_beg_y = floor(y + sin(deg) * (radius >> s_radius_ratio));

            draw_ray(ray_beg_x, ray_beg_y, ray_end_x, ray_end_y);
            draw_ray(ray_beg_x, 2 * y - ray_beg_y, ray_end_x, 2 * y - ray_end_y);
            draw_ray(2 * x - ray_beg_x, ray_beg_y, 2 * x - ray_end_x, ray_end_y);
            draw_ray(2 * x - ray_beg_x, 2 * y - ray_beg_y, 2 * x - ray_end_x, 2 * y - ray_end_y);
        }

        draw_hero_sprite(x-15, y-22, hero_state);
        update_graphics();

        }
    }

    close_game();
	return 0;
}

