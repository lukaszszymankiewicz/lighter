#include "gfx.h"
#include "primitives.h"
#include "intersection.h"
#include "geometry.h"
#include "vertex.h"
#include "obstacle.h"

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

void GFX_draw_scanline(obstacle_t* lines, int y, int clr){
    int x;
    int n=0;
    int last_intersection;

    if (lines){
        obstacle_t* ptr = lines;
        x_intersection_t* points = NULL;

        while(ptr){
            n++;
            x = GEO_calc_intersection_with_slope(y, ptr->x1, ptr->y1, ptr->slope);
            INTSC_insert(&points, x);
            ptr=ptr->next;
        }

        // if no intersection found this scanline should be all black 
        if (n==0) {
            // GFX_draw_colored_line(0, y, SCREEN_WIDTH, y, 0, 0, 0, 100);
            return;
        }

        // dark segment from left side of screen to first intersection with scanline
        if (points->x > 1) {
            // GFX_draw_colored_line(0, y, points->x, y, 0, 0, 0, 100);
        }

        // dark segment from right side of screen to last intersection with scanline
        last_intersection = INTSC_get_last(points);

        if (last_intersection < SCREEN_WIDTH-1) {
            // GFX_draw_colored_line(last_intersection, y, SCREEN_WIDTH, y, 0, 0, 0, 100);
        }

        // rest of the lines (light and dark sectors)
        if (n>2)
        {
            for (int z=1; z<n-1; z+=2)
            {
                GFX_draw_colored_line(points->x, y, points->next->x, y, 169, 169, 169, clr); 
                points=points->next;

                // GFX_draw_colored_line(points->x, y, points->next->x, y, 0, 0, 0, 255); 
                points=points->next;
            }
        }

        // last of the dark sector from intesection points
        // GFX_draw_colored_line(points->x, y, points->next->x, y, 169, 169, 169, clr); 
    }
}

void GFX_draw_polygon(vertex_t* poly, int clr){
    obstacle_t* not_drawn_yet = VRTX_get_obstacles_from_polygon(poly); 
    obstacle_t* current_draw = NULL;
    obstacle_t* obstacle_ptr = NULL;
    obstacle_t* candidates = NULL;
    
    printf("vertices: \n");

    for (vertex_t* ptr = poly; ptr; ptr=ptr->next){
        printf("(%i, %i) \n", ptr->x, ptr->y);
    }

    for (int y=0; y<SCREEN_HEIGHT; y++){

        // delete light obstacles current drawn scan_y must be higher than y of any point
        obstacle_ptr = current_draw;

        while(obstacle_ptr){
            OBS_delete(&current_draw, y);
            obstacle_ptr=obstacle_ptr->next;
        }

        // get candidates to draw
        candidates = OBS_find_candidates(&not_drawn_yet, y);

        // add candidates to current draw
        OBS_merge(&current_draw, candidates);       

        if (!not_drawn_yet && !current_draw){
            break;
        }

        GFX_draw_scanline(current_draw, y, clr);
    }
}

