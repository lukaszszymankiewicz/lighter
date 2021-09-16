#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "math.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // preparations
    vertex_t *poly = NULL;
    ray_t ray;

    // left
    ray = (ray_t){100, 100, 50, 100};
    VRTX_insert(&poly, &ray);

    // right
    ray = (ray_t){100, 100, 150, 110};
    VRTX_insert(&poly, &ray);

    // down
    ray = (ray_t){100, 100, 100, 120};
    VRTX_insert(&poly, &ray);

    // up
    ray = (ray_t){100, 100, 100, 80};
    VRTX_insert(&poly, &ray);


    // SDL part
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        window = SDL_CreateWindow(
            "Lighter",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            640,
            480,
            0
        );
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

                GFX_draw_polygon(renderer, poly);

                SDL_RenderPresent(renderer);
                SDL_Delay(100);

                SDL_Event event;
                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                }
            }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}
