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
        GAME_NAME,
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

void GFX_draw_colored_line(
    float x1,
    float y1,
    float x2,
    float y2,
    int   r,
    int   g,
    int   b,
    int   a
) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};

void GFX_draw_colored_rect(
    float x1,
    float y1,
    float w,
    float h,
    int   r,
    int   g,
    int   b,
    int   a
){
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_Rect rect = (SDL_Rect){x1, y1, w, h};
    SDL_RenderFillRect(renderer, &rect);
};


void GFX_draw_light_sectors_in_scanline(
    x_intersection_t  *points,
    int                y,
    int                n,
    int                r,
    int                g,
    int                b,
    int                a
){
    x_intersection_t* ptr = NULL;

    if (n==2) {
        GFX_draw_colored_line(points->x, y, points->next->x, y, r, g, b, a);
    }

    else if (n>2) {
        ptr = points;

        while (ptr->next){
            GFX_draw_colored_line(ptr->x, y, ptr->next->x, y, r, g, b, a);
            ptr=ptr->next;
            if (ptr->next == NULL){break;}
            ptr=ptr->next;
        }
    }
}

void GFX_draw_dark_sectors_in_scanline(
    x_intersection_t  *points,
    int                y,
    int                n
){
    x_intersection_t* ptr = NULL;

    int last_intersection = INTSC_get_last(points);

    // dark segment from left side of screen to first intersection with scanline
    if (points->x > 1) {
        GFX_draw_colored_line(
            0, y, points->x, y,
            DEFAULT_DARK_R, DEFAULT_DARK_G, DEFAULT_DARK_B, DEFAULT_DARK_A
        );
    }

    // dark segment from right side of screen to last intersection with scanline
    if (last_intersection < SCREEN_WIDTH-1) {
        GFX_draw_colored_line(
            last_intersection, y, SCREEN_WIDTH, y,
            DEFAULT_DARK_R, DEFAULT_DARK_G, DEFAULT_DARK_B, DEFAULT_DARK_A
        );
    }

    // rest of the lines
    if (n>2) {
        ptr = points;

        while (ptr->next){
            GFX_draw_colored_line(
                ptr->x, y, ptr->next->x, y,
                DEFAULT_DARK_R, DEFAULT_DARK_G, DEFAULT_DARK_B, DEFAULT_DARK_A
            );
            ptr=ptr->next;
        }
    }
}

x_intersection_t* GFX_calc_intersections_in_scanline(
    obstacle_t *obstacles,
    int         y,
    int        *n
){
    x_intersection_t *intersections = NULL;
    obstacle_t       *ptr           = NULL;
    int               x;

    ptr = obstacles;

    while(ptr){
        // line in perpendicular to y-axis
        if (ptr->y1 == ptr->y2){
            INTSC_insert(&intersections, ptr->x1);
            INTSC_insert(&intersections, ptr->x2);
            ptr=ptr->next;
            (*n)=(*n)+2;
        }
        else {
            x = GEO_calc_intersection_with_slope(y, ptr->x1, ptr->y1, ptr->slope);
            INTSC_insert(&intersections, x);
            ptr=ptr->next;
            (*n)++;
        }
    }

    return intersections;
}

void GFX_draw_scanline(
    obstacle_t    *lines,
    int            y,
    int r,
    int g,
    int b,
    int a
){
    int n=0;
    x_intersection_t* intersections = NULL;

    intersections = GFX_calc_intersections_in_scanline(lines, y, &n);

    if (intersections){
        GFX_draw_light_sectors_in_scanline(intersections, y, n, r, g, b, a);
        GFX_draw_dark_sectors_in_scanline(intersections->next, y, n);
        INTSC_free(intersections);
    }
}

// function, firstly checks where the polygon "begins" (vertex with highest y coord value) and draw
// dark rect from y=0 up to y of such vertex. Then iterating by every y up to "end" of polygon
// (vertex with lowest y coord value), polygon segments which will be drawn is chosen (and these
// ones which won`t be needed are discarded), intersection points are calculated and then "dark and
// "light" sectors are filled.
void GFX_draw_light_polygon(
    vertex_t     *poly,
    int r, int g, int b, int a
){
    int y                     = VRTX_highest_y(poly);
    obstacle_t* not_drawn_yet = OBS_get_obstacles_from_polygon(poly); 
    obstacle_t* current_draw  = NULL;
    obstacle_t* obstacle_ptr  = NULL;
    obstacle_t* candidates    = NULL;

    if (y > 0) {
        GFX_draw_colored_rect(0, 0, SCREEN_WIDTH, y, DEFAULT_DARK_R, DEFAULT_DARK_G, DEFAULT_DARK_B, DEFAULT_DARK_A);
    }

    while(y<SCREEN_HEIGHT) {
        obstacle_ptr = current_draw;

        // delete light obstacles current drawn scan_y must be higher than y of any point
        while(obstacle_ptr){
            OBS_delete(&current_draw, y);
            obstacle_ptr=obstacle_ptr->next;
        }

        // get candidates to draw
        candidates = OBS_find_candidates(&not_drawn_yet, y);

        // add candidates to current draw
        OBS_merge(&current_draw, candidates);       

        // if there isn`t anything to draw or anything to be drawn in future - stop
        if (!not_drawn_yet && !current_draw){ break; }

        GFX_draw_scanline(current_draw, y, r, g, b, a);
        y++;
    }

    // fill the rest with darkness (sounds deep)
    GFX_draw_colored_rect(0, y, SCREEN_WIDTH, y, DEFAULT_DARK_R, DEFAULT_DARK_G, DEFAULT_DARK_B, DEFAULT_DARK_A);
}

