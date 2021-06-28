#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "tile.h"
#include "config.h"

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;


SDL_Window* GFX_init_window(int w, int h) 
{
    window = SDL_CreateWindow("Lighter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (window == NULL) 
    {
        printf("window cannot be created");
    }
    return window;
};

void GFX_init_video() 
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("video canno be initialized!");
    };
};

SDL_Renderer* GFX_init_renderer(SDL_Window* window) 
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) 
    {
        printf("renderer cannot be initialized!");
    }
    return renderer;
};

void GFX_init_png() 
{
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) 
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
};

void GFX_enable_alpha_bleeding() 
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
};

int GFX_init_graphics(int width, int height) 
{
    GFX_init_video();
    window = GFX_init_window(width, height);
    renderer = GFX_init_renderer(window);
    GFX_init_png();
    GFX_enable_alpha_bleeding();

    return 1;
};

void GFX_free() 
{
    window = NULL;
    renderer = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    SDL_Quit();
};

void GFX_clear_screen() 
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
};

void GFX_update() 
{
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
};

void GFX_draw_colored_line(float x1, float y1, float x2, float y2, int r, int g, int b, int a) 
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};

