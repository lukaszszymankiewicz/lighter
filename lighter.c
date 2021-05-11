#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
 
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define TICK_INTERVAL 30

enum sprites {
    HERO_SPRITE_FRONT,
    HERO_SPRITE_BACK,
    HERO_SPRITE_LEFT,
    HERO_SPRITE_RIGHT,
    SPIRTES_TOTAL,
};

// global declarations
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* current_sprite = NULL;
SDL_Surface* sprites_collection[SPIRTES_TOTAL];

// headers
SDL_Surface* load_sprite(char *filepath);
int init_game();
void load_frames();
void close_game();

int init_game() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Fatal Error!");
        return 0;
    }

    else {
        window = SDL_CreateWindow(
           "Lighter",
           SDL_WINDOWPOS_UNDEFINED,
           SDL_WINDOWPOS_UNDEFINED,
           SCREEN_WIDTH,
           SCREEN_HEIGHT,
           SDL_WINDOW_SHOWN
        );
        surface = SDL_GetWindowSurface(window);

    }
    return 1;
}

SDL_Surface* load_sprite(char *filepath) {
    SDL_Surface* sprite = SDL_LoadBMP(filepath);

    if(sprite == NULL) {
        printf("unable to locate image: %s", filepath);
    }

    return sprite;
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

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

int main(int argc, char* args[]) {
    int loop = 1;
    SDL_Event event;

    load_frames();
    init_game();

    while(loop) {

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

        //Apply the image stretched
        SDL_Rect stretchRect;

        stretchRect.x = 0;
        stretchRect.y = 0;
        stretchRect.w = 31;
        stretchRect.h = 46;

        surface = SDL_GetWindowSurface(window);
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);

        SDL_BlitScaled(current_sprite, NULL, surface, &stretchRect);
        SDL_UpdateWindowSurface(window);

        }
    }

    close_game();
	return 0;
}
