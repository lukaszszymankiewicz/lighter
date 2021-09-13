#include "gfx.h"
#include "primitives.h"
#include "intersection.h"
#include "geometry.h"
#include "vertex.h"

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;


SDL_Window* GFX_init_window(int w, int h) {
    window = SDL_CreateWindow(
        "Lighter",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1366,
        768,
        SDL_WINDOW_FULLSCREEN
    );

    if (window == NULL) {
        printf("window cannot be created");
    }
    return window;
};

void GFX_init_video() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("video canno be initialized!");
    };
};

SDL_Renderer* GFX_init_renderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (renderer == NULL) {
        printf("renderer cannot be initialized!");
    }
    return renderer;
};

void GFX_init_png() {
    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
};

void GFX_enable_alpha_bleeding() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
};

int GFX_init_graphics(int width, int height) {
    GFX_init_video();
    window = GFX_init_window(width, height);
    renderer = GFX_init_renderer(window);
    GFX_init_png();
    GFX_enable_alpha_bleeding();

    return 1;
};

void GFX_free() {
    window = NULL;
    renderer = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    SDL_Quit();
};

void GFX_clear_screen() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
};

void GFX_blender() {
    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 10);
    SDL_RenderClear(renderer);
};

void GFX_update() {
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
};

void GFX_draw_colored_line(float x1, float y1, float x2, float y2, int r, int g, int b, int a) {
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

// Fills screen with light and dark lines. Inside of the light polygon is filled with bright lines
// (with high alpha channel value) and the rest (outside of the light polygon) is filled with dark
// lines (with very low alpha channel value). Function assumes that points of light polygon are
// sorted in ascending order. To sort point value of an angle which given point make with player
// position is used.
void GFX_fill_lightpoly(vertex_t* vertices, int clr) {
    int scan_y=0;
    int n;
    int last_x_intersection;
    float x_inter;

    vertex_t* current_vertices = NULL;
    vertex_t* ptr = NULL;
    vertex_t* waiting_vertices = NULL;
    x_intersection_t* intersections;

    // we start with all vertices waiting to be drawn
    waiting_vertices = vertices;

    // Scan every y for seach of colision with light polygon.
    for (scan_y=0; scan_y<SCREEN_HEIGHT; scan_y++) {
        intersections=NULL;   // clean the intersections
        n=0;                  // number of itersection

        // delete old vertices
        ptr = current_vertices;
        while(ptr && ptr->next) {
            if (ptr->y < scan_y && ptr->next->y < scan_y){
                if (!(ptr->prev)) {
                    ptr->next->prev = NULL;
                }
                else {
                    ptr->prev->next = ptr->next;
                    ptr->next->prev = ptr->prev;
                }
                free(ptr);
            }
            ptr=ptr->next;
        }

        // add new vertices to draw
        ptr = waiting_vertices;
        while(ptr && ptr->next) {
            if (ptr->y == scan_y || ptr->next->y == scan_y) {
                VRTX_push(&current_vertices, ptr);

                if (!(ptr->prev)) {
                    ptr->next->prev = NULL;
                }
                else if (!(ptr->next)) {
                    ptr->prev->next = ptr->next;
                }
                else {
                    ptr->prev->next = ptr->next;
                    ptr->next->prev = ptr->prev;
                }
                free(ptr);

            }
            ptr=ptr->next;
        }

        if (!current_vertices){
            if (!waiting_vertices){
                GFX_draw_colored_line(0, scan_y, SCREEN_WIDTH, scan_y, 0, 0, 0, 100);
                break;
            }
            else {
                continue;
            }
        }

        // now we got the current_vertices which colision are now calculated
        ptr = current_vertices;

        while(ptr) {
            x_inter = ptr->x + (scan_y - ptr->y) * ptr->slope;
            INTSC_insert(&intersections, x_inter);
            n++;
            ptr=ptr->next;
        }

        if (n==1){
            continue;
        }

        // dark segment from left side of screen to first intersection with scanline
        if (intersections->x > 1) {
            // GFX_draw_colored_line(0, scan_y, intersections->x, scan_y, 0, 0, 0, 100);
        }

        // dark segment from right side of screen to last intersection with scanline
        last_x_intersection = INTSC_get_last(intersections);
        if (last_x_intersection < SCREEN_WIDTH-1) { 
            // GFX_draw_colored_line(last_x_intersection, scan_y, SCREEN_WIDTH, scan_y, 0, 0, 0, 100); 
        }

        // rest of the lines (light and dark sectors)
        if (n>2) {
            for (int z=1; z<n-1; z+=2) {
                GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, 255, 255, 255, 200); 
                intersections=intersections->next;

                // GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, 0, 0, 0, 100); 
                intersections=intersections->next;
            }
            // last of the dark sector from intesection points
            // GFX_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, 169, 169, 169, clr); 
        }


        INTSC_free(intersections);
    }
}
