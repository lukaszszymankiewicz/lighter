#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>
#include "draw.h"

#define pi 3.14

SDL_Window* create_window(int width, int height);
void init_video();
SDL_Renderer* init_renderer(SDL_Window* window);
void init_png();
int init_graphics(int width, int height);
Texture *init_texture(char *filepath);
void enable_alpha_bleeding();
void render_texture(Texture* texture, SDL_Rect* clip, int x, int y);

Texture* hero_sprites;
SDL_Rect hero_texture_clips[ 4 ];

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;


// SETUP
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
    enable_alpha_bleeding();

    return 1;
};

Texture *init_texture(char *filepath) {
	SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = NULL;

    loaded_surface = IMG_Load(filepath);

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
    hero_texture_clips[ 3 ].y =  0;
    hero_texture_clips[ 2 ].w = 30;
    hero_texture_clips[ 2 ].h = 45;

    //Set bottom right sprite
    hero_texture_clips[ 3 ].x = 93;
    hero_texture_clips[ 3 ].y =  0;
    hero_texture_clips[ 3 ].w = 30;
    hero_texture_clips[ 3 ].h = 45;
};

void enable_alpha_bleeding() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
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

// DRAW
void draw_hero_sprite(int x, int y, int hero_state) {
    render_texture(hero_sprites, &hero_texture_clips[hero_state], x, y);
};

void render_texture(Texture* texture, SDL_Rect* clip, int x, int y) {
    SDL_Rect render_quad = {x, y, texture->width, texture->height};

    if(clip != NULL){
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopy(renderer, texture->surface, clip, &render_quad);
};

void clear_screen() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
};

void update_graphics() {
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
};

void draw_wall(int x0, int y0, int x1, int y1) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);     
    SDL_RenderDrawLine(renderer, x0, y0, x1, y1);
};

void draw_walls(Polygon *obstacles) {
    for(int i=0; i<obstacles->len; i++)
    {
        draw_wall(
            obstacles->lines[i].x1, obstacles->lines[i].y1,
            obstacles->lines[i].x2, obstacles->lines[i].y2
        );
    }
};

IntersectionPoint find_intersection_point(Line ray, Line wall)
{
    IntersectionPoint invalid_point = {0, 0, 0, false};

	float ray_dx = ray.x2 - ray.x1;
	float ray_dy = ray.y2 - ray.y1;
	float wall_dx = wall.x2 - wall.x1;
	float wall_dy = wall.y2 - wall.y1;

	float r_mag = sqrt(ray_dx * ray_dx + ray_dy * ray_dy);
	float s_mag = sqrt(wall_dx * wall_dx + wall_dy * wall_dy);

	if(ray_dx/r_mag==wall_dx/s_mag && ray_dy/r_mag==wall_dy/s_mag){
		return invalid_point;
    }

	float T2 = (ray_dx*(wall.y1-ray.y1) + ray_dy*(ray.x1-wall.x1))/(wall_dx*ray_dy - wall_dy*ray_dx);
	float T1 = (wall.x1+wall_dx*T2-ray.x1)/ray_dx;

	if (T1<0 || T2<0 || T2>1) {
        return invalid_point;
    }

    IntersectionPoint valid_point = {
		ray.x1 + ray_dx * T1,
		ray.y1 + ray_dy * T1,
		T1,
        true,
    };

    return valid_point;
};

IntersectionPoint choose_closest_point(IntersectionPoint closest, IntersectionPoint candidate){
    if (closest.found == false && candidate.found == true) {
        return candidate;
    }
    else if (closest.found == true && candidate.found == true && closest.T1 > candidate.T1) {
        return candidate;
    }
    else {
        return closest;
    }
};

void draw_rays(float st_x, float st_y, Polygon *obstacles){ 
    int resolution = 50;

    for(float angle=0.0; angle<pi*2; angle += (pi*2)/resolution)
    {
        IntersectionPoint closest = {0, 0, 0, false};

        Line ray = {st_x, st_y, st_x + cos(angle), st_y + sin(angle)};

        for (int j = 0; j<obstacles->len; j++) {
            IntersectionPoint candidate = find_intersection_point(ray, obstacles->lines[j]);
            closest = choose_closest_point(closest, candidate);
        }

        if (closest.found == true) {
            draw_wall(st_x, st_y, closest.x, closest.y);
        }
    }
};
