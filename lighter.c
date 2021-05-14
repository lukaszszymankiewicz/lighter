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


int radius = 100;
float pi = 3.14;

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
    sprites_collection[HERO_SPRITE_FRONT] = load_sprite("player_front.bmp");
    sprites_collection[HERO_SPRITE_BACK] = load_sprite("player_back.bmp");
    sprites_collection[HERO_SPRITE_LEFT] = load_sprite("player_left.bmp");
    sprites_collection[HERO_SPRITE_RIGHT] = load_sprite("player_right.bmp");

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

    float QuaterRadDeg = 0.5 * pi / (2 * radius - 3);
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

        int hero_width = 10;
        int hero_height = 15;
        int scale = 3;
        int correction = 1;

        int hero_x = SCREEN_HEIGHT / 2;
        int hero_y = SCREEN_WIDTH / 2;

        // show light
        for (int i=0; i< 2*radius-2; i++) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);     
            float RadDeg = i * QuaterRadDeg;

            int new_x = floor(hero_x + cos(RadDeg) * radius);
            int new_y = floor(hero_y + sin(RadDeg) * radius);

            SDL_RenderDrawLine(renderer, hero_x, hero_y, new_x, new_y);
            SDL_RenderDrawLine(renderer, hero_x, hero_y, new_x, 2 * hero_y - new_y);
            SDL_RenderDrawLine(renderer, hero_x, hero_y, 2 * hero_x - new_x, new_y);
            SDL_RenderDrawLine(renderer, hero_x, hero_y, 2 * hero_x - new_x, 2 * hero_y - new_y);

            SDL_RenderPresent(renderer);
        }

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
