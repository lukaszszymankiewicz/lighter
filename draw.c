#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "draw.h"


Texture* hero_sprites;
SDL_Rect hero_texture_clips[ 4 ];

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;

SDL_Window* create_window(int width, int height) {
    window = SDL_CreateWindow(
       "Lighter",
       SDL_WINDOWPOS_UNDEFINED,
       SDL_WINDOWPOS_UNDEFINED,
       width,
       height,
       SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        printf("window cannot be created");
    }
    return window;
};

void init_video() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("video canno be initialized!");
    };
};

SDL_Renderer* init_renderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        printf("renderer cannot be initialized!");
    }
    return renderer;
};

void init_png() {
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
};

int init_graphics(int width, int height) {
    init_video();
    window = create_window(width, height);
    renderer = init_renderer(window);
    init_png();

    return 1;
};

Texture *init_texture(char *filepath) {
	SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = NULL;

    loaded_surface = IMG_Load(filepath);

    // this will set bg transparency color

    // background color = 128 255 255
    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x80, 0xFF, 0xFF));

    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

    Texture *p = malloc(sizeof(Texture));

    p->surface = new_texture;
    p->width = loaded_surface->w;
    p->height = loaded_surface->h;

    SDL_FreeSurface(loaded_surface);

    return p;
};

void load_frames() {
    hero_sprites = init_texture("sprites/hero.png");

    hero_texture_clips[ 0 ].x =  0;
    hero_texture_clips[ 0 ].y =  0;
    hero_texture_clips[ 0 ].w = 30;
    hero_texture_clips[ 0 ].h = 45;

    //Set top right sprite
    hero_texture_clips[ 1 ].x = 31;
    hero_texture_clips[ 1 ].y =  0;
    hero_texture_clips[ 1 ].w = 30;
    hero_texture_clips[ 1 ].h = 45;
    
    //Set bottom left sprite
    hero_texture_clips[ 2 ].x = 62;
    hero_texture_clips[ 2 ].y =  0;
    hero_texture_clips[ 2 ].w = 30;
    hero_texture_clips[ 2 ].h = 45;

    //Set bottom right sprite
    hero_texture_clips[ 3 ].x = 93;
    hero_texture_clips[ 3 ].y =  0;
    hero_texture_clips[ 3 ].w = 30;
    hero_texture_clips[ 3 ].h = 45;
};

void free_texture(Texture *texture) {
    SDL_DestroyTexture(texture->surface);
    free(texture);
};

void free_graphics() {
    free_texture(hero_sprites);

    window = NULL;
    renderer = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    SDL_Quit();
};


void render_texture(Texture* texture, SDL_Rect* clip, int x, int y) {
    SDL_Rect render_quad = {x, y, texture->width, texture->height};

    if(clip != NULL){
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }
    SDL_RenderCopy(renderer, texture->surface, clip, &render_quad);

};

void draw_hero_sprite(int x, int y, int hero_state) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    render_texture(hero_sprites, &hero_texture_clips[hero_state], x, y);
};

void clear_screen() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
};

void update_graphics() {
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
};

void draw_ray(int x1, int y1, int x2, int y2) {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);     
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};
