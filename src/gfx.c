#include <SDL2/SDL_image.h>
#include "config.h"

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;


SDL_Window* GFX_init_window(int w, int h) 
{
    window = SDL_CreateWindow(
        "Lighter",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1366,
        768,
        SDL_WINDOW_FULLSCREEN
    );

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
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

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
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
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

void GFX_blender() 
{
    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 10);
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


// point are sorted by y increasingly, we could use full more sofisticated sorting, but here it is
// so basic that this task handled easily by if-conditions. Points should be sorted in a way that y1
// < y2 < y3 (x-coords are not taken into sorting).
void GFX_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a){
    int swapx;
    int swapy;
    
    if (y1 > y3) {swapx=x3; swapy=y3; x3=x1; y3=y1; x1=swapx; y1=swapy;}
    if (y2 > y3) {swapx=x3; swapy=y3; x3=x2; y3=y2; x2=swapx; y2=swapy;}
    if (y1 > y2) {swapx=x2, swapy=y2; x2=x1; y2=y1; x1=swapx; y1=swapy;}
    
    for (int y=y1; y<y2; y++) {
        float linex1 = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        float linex2 = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        GFX_draw_colored_line(linex1, y, linex2, y, r, g, b, a);
    }
    for (int y=y2; y<y3; y++) {
        float linex1 = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
        float linex2 = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        GFX_draw_colored_line(linex1, y, linex2, y, r, g, b, a);
    }
};
