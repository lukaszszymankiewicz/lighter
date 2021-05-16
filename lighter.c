#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 512

enum sprites {
    HERO_SPRITE_FRONT,
    HERO_SPRITE_BACK,
    HERO_SPRITE_LEFT,
    HERO_SPRITE_RIGHT,
    SPIRTES_TOTAL,
};

// headers
SDL_Surface* load_sprite(char *filepath);
int init_game();
void load_frames();
void close_game();

// global declarations
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* current_sprite = NULL;
SDL_Surface* sprites_collection[SPIRTES_TOTAL];
SDL_Surface* loaded_sprite = NULL;
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

SDL_Surface* load_sprite(char *filepath) {
    SDL_Surface* loaded_sprite = IMG_Load(filepath);

    if(loaded_sprite == NULL) {
        printf("unable to locate image: %s", filepath);
    }

    return loaded_sprite;
}

void load_frames() {
    sprites_collection[HERO_SPRITE_FRONT] = load_sprite("sprites/player_front.bmp");
    sprites_collection[HERO_SPRITE_BACK] = load_sprite("sprites/player_back.bmp");
    sprites_collection[HERO_SPRITE_LEFT] = load_sprite("sprites/player_left.bmp");
    sprites_collection[HERO_SPRITE_RIGHT] = load_sprite("sprites/player_right.bmp");

    current_sprite = sprites_collection[HERO_SPRITE_BACK];
}

void close_game() {
    SDL_FreeSurface(sprites_collection[HERO_SPRITE_FRONT]);
    SDL_FreeSurface(sprites_collection[HERO_SPRITE_BACK]);
    SDL_FreeSurface(sprites_collection[HERO_SPRITE_LEFT]);
    SDL_FreeSurface(sprites_collection[HERO_SPRITE_RIGHT]);

    window = NULL;
    renderer = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    SDL_Quit();
}


int main(int argc, char* args[]) {
    int loop = 1;

    int hero_width = 10;
    int hero_height = 15;
    int scale = 3;
    int correction = 1;

    int hero_x = SCREEN_HEIGHT / 2;
    int hero_y = SCREEN_WIDTH / 2;

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

            int ray_end_x = floor(hero_x + cos(deg) * radius);
            int ray_end_y = floor(hero_y + sin(deg) * radius);

            int ray_beg_x = floor(hero_x + cos(deg) * (radius >> s_radius_ratio));
            int ray_beg_y = floor(hero_y + sin(deg) * (radius >> s_radius_ratio));

            SDL_RenderDrawLine(renderer, ray_beg_x, ray_beg_y, ray_end_x, ray_end_y);
            SDL_RenderDrawLine(renderer, ray_beg_x, 2 * hero_y - ray_beg_y, ray_end_x, 2 * hero_y - ray_end_y);
            SDL_RenderDrawLine(renderer, 2 * hero_x - ray_beg_x, ray_beg_y, 2 * hero_x - ray_end_x, ray_end_y);
            SDL_RenderDrawLine(renderer, 2 * hero_x - ray_beg_x, 2 * hero_y - ray_beg_y, 2 * hero_x - ray_end_x, 2 * hero_y - ray_end_y);
        }

        SDL_RenderPresent(renderer);

        // Sprites
        surface = SDL_GetWindowSurface(window);
        SDL_Rect stretchRect;

        stretchRect.w = hero_width * scale + correction;  // 31
        stretchRect.h = hero_height * scale + correction; // 46
        stretchRect.x = SCREEN_HEIGHT / 2 - (hero_width * scale) / 2;
        stretchRect.y = SCREEN_WIDTH / 2 - (hero_height * scale) / 2;

        SDL_BlitScaled(current_sprite, NULL, surface, &stretchRect);
        SDL_UpdateWindowSurface(window);

        }
    }

    close_game();
	return 0;
}
