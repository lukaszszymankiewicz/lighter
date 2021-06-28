#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef GFX_H
#define GFX_H

extern SDL_Window* window;
extern SDL_Surface* surface;
extern SDL_Renderer* renderer;

int GFX_init_graphics(int width, int height);
void GFX_free();
void GFX_clear_screen();
void GFX_update();
void GFX_draw_colored_line(float x1, float y1, float x2, float y2, int r, int g, int b, int a);

#endif
