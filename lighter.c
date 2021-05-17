#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include "tex.h"

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

enum sprites {
    HERO_SPRITE_FRONT,
    HERO_SPRITE_BACK,
    HERO_SPRITE_LEFT,
    HERO_SPRITE_RIGHT,
    SPIRTES_TOTAL,
};

int init_game();
void load_frames();
void close_game();

// global declarations
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
Texture* current_sprite = NULL;
Texture* sprites_collection[SPIRTES_TOTAL];
SDL_Renderer* renderer = NULL;

int radius = 128;
int s_radius_ratio = 2;
float half_pi = 1.57;

int init_game() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Fatal Error!");
        return 0;
    }

    // initializing window
    window = SDL_CreateWindow(
       "Lighter",
       SDL_WINDOWPOS_UNDEFINED,
       SDL_WINDOWPOS_UNDEFINED,
       SCREEN_WIDTH,
       SCREEN_HEIGHT,
       SDL_WINDOW_SHOWN
    );

    // initializing renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // initializing PNG
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
        printf("Fatal Error!");
        return 0;
    }

    return 1;
}

void load_frames() {
    sprites_collection[HERO_SPRITE_FRONT] = init_texture("sprites/player_front.bmp", renderer);
    sprites_collection[HERO_SPRITE_BACK] = init_texture("sprites/player_back.bmp", renderer);
    sprites_collection[HERO_SPRITE_LEFT] = init_texture("sprites/player_left.bmp", renderer);
    sprites_collection[HERO_SPRITE_RIGHT] = init_texture("sprites/player_right.bmp", renderer);

    current_sprite = sprites_collection[HERO_SPRITE_BACK];
}

void close_game() {
    free_texture(sprites_collection[HERO_SPRITE_FRONT]);
    free_texture(sprites_collection[HERO_SPRITE_BACK]);
    free_texture(sprites_collection[HERO_SPRITE_LEFT]);
    free_texture(sprites_collection[HERO_SPRITE_RIGHT]);

    window = NULL;
    renderer = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    SDL_Quit();
}


int main(int argc, char* args[]) {
    int loop = 1;
    int scale = 3;

    int x = SCREEN_HEIGHT / 2;
    int y = SCREEN_WIDTH / 2;
    int resolution = 128;

    // because we calculate hero rays for quater of full circle (other are just symetries)
    float deg_res = half_pi / 2 / resolution; 

    SDL_Event event;
    init_game();
    load_frames();

    while(loop) {

        // handle events
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                loop = 0;
            }

            else if(event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                    current_sprite = sprites_collection[HERO_SPRITE_BACK];
                    break;

                    case SDLK_DOWN:
                    current_sprite = sprites_collection[HERO_SPRITE_FRONT];
                    break;

                    case SDLK_RIGHT:
                    current_sprite = sprites_collection[HERO_SPRITE_RIGHT];
                    break;

                    case SDLK_LEFT:
                    current_sprite = sprites_collection[HERO_SPRITE_LEFT];
                    break;
                }
            }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // render light
        for (float deg=0; deg<half_pi; deg+=deg_res) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);     

            int ray_end_x = floor(x + cos(deg) * radius);
            int ray_end_y = floor(y + sin(deg) * radius);

            int ray_beg_x = floor(x + cos(deg) * (radius >> s_radius_ratio));
            int ray_beg_y = floor(y + sin(deg) * (radius >> s_radius_ratio));

            SDL_RenderDrawLine(renderer, ray_beg_x, ray_beg_y, ray_end_x, ray_end_y);
            SDL_RenderDrawLine(renderer, ray_beg_x, 2 * y - ray_beg_y, ray_end_x, 2 * y - ray_end_y);
            SDL_RenderDrawLine(renderer, 2 * x - ray_beg_x, ray_beg_y, 2 * x - ray_end_x, ray_end_y);
            SDL_RenderDrawLine(renderer, 2 * x - ray_beg_x, 2 * y - ray_beg_y, 2 * x - ray_end_x, 2 * y - ray_end_y);
        }

        // Sprites
        surface = SDL_GetWindowSurface(window);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

        render_texture(renderer, current_sprite, scale, x-15, y-22);

        SDL_RenderPresent(renderer);

        SDL_UpdateWindowSurface(window);
        }
    }

    close_game();
	return 0;
}
