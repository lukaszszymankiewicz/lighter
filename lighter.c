#include <SDL2/SDL.h>
 
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Surface* player_frame = NULL;

int init_game(){
    if(SDL_Init( SDL_INIT_VIDEO ) < 0) {
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

int load_frames() {
    player_frame = SDL_LoadBMP("player.bmp");

    if(player_frame == NULL) {
        printf("unable to locate image!");
        return 0;
    }
    printf("All frames loaded sucesfully!");
    return 1;
}

void close_game() {
    SDL_FreeSurface( player_frame );
    player_frame = NULL;
    SDL_DestroyWindow( window );
    window = NULL;
    SDL_Quit();
}

int main( int argc, char* args[] ) {
    init_game();
    load_frames();
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFA, 0xFF));
    SDL_BlitSurface(player_frame, NULL, surface, NULL);
    SDL_UpdateWindowSurface(window);
    SDL_Delay(2000);
    close_game();
}
