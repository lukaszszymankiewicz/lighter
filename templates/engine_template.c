#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window   *window         = NULL;
SDL_Renderer *renderer       = NULL;

typedef struct texture {
    SDL_Texture *surface;
    int          width;
    int          height;
} texture_t;

texture_t* GFX_read_texture(
    char *filepath
) {
	SDL_Texture *new_texture    = NULL;
    SDL_Surface *loaded_surface = NULL;

    loaded_surface              = IMG_Load(filepath);

    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x80, 0xFF, 0xFF));
    new_texture  = SDL_CreateTextureFromSurface(renderer, loaded_surface);
    texture_t* p = malloc(sizeof(texture_t));

    p->surface = new_texture;
    p->width   = loaded_surface->w;
    p->height  = loaded_surface->h;

    SDL_FreeSurface(loaded_surface);

    return p;
};

void GFX_render_texture(
    texture_t *texture,   // Texture to be rendered
    SDL_Rect  *clip,      // rect from texture to be rendered
    int        x,         // x coord of screen to have texture rendered
    int        y,         // y coord of screen to have texture rendered
    bool       flip       // indiaction if texture should be flipped horizontally
) {
    SDL_Rect render_quad = {x, y, texture->width, texture->height};
    SDL_RendererFlip flip_tex;

    // if clip is not given render whole texture
    if(clip != NULL) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }
    
    if (flip) {
        flip_tex = SDL_FLIP_HORIZONTAL; 
    }
    else {
        flip_tex = SDL_FLIP_NONE; 
    }

    SDL_RenderCopyEx(renderer, texture->surface, clip, &render_quad, 0, NULL, flip_tex);
};

int main(
    int argc, char* args[]
) {


    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(
        "temp",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1366,
        768,
        SDL_WINDOW_FULLSCREEN
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, 320, 200);

    int loop = 1;
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }

    while(loop < 10) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(renderer);

        // Render here

        SDL_RenderPresent(renderer);
        SDL_Delay(100);
        SDL_UpdateWindowSurface(window);
        loop++;
    }
    SDL_DestroyWindow(window);
    return 1;
}
