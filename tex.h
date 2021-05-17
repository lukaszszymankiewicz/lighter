#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    SDL_Texture* surface;
    int width;
    int height;
} Texture;

Texture *init_texture(char *filepath, SDL_Renderer* renderer);
void free_texture(Texture *texture);
void render_texture(SDL_Renderer* renderer, Texture* texture, int scale, int x, int y);
