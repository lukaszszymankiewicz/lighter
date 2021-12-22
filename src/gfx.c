#include "global.h"
#include "gfx.h"
#include "primitives.h"
#include "sorted_list.h"
#include "geometry.h"
#include "vertex.h"
#include "segment.h"


SDL_Window   *window         = NULL;
SDL_Renderer *renderer       = NULL;
SDL_Texture  *screen_texture = NULL;

uint32_t     *lightbuffer    = NULL;
uint32_t     *shadowbuffer   = NULL;

void GFX_fill_shadowbuffer(
    uint32_t  color,
    int       x,
    int       y,
    int       power
) {
    shadowbuffer[x+y*SCREEN_WIDTH] = color | power;
}

void GFX_fill_mesh_shadowbuffer(
    uint32_t  color,
    int       x,
    int       y,
    int       power
) {
    static int mesh = 4;
    shadowbuffer[x+y*SCREEN_WIDTH] = (color | power) * !((x+y) % mesh + (y % 3));
}

void GFX_fill_lightbuffer(
    uint32_t  color,
    int       x,
    int       y,
    int       power
) {
    lightbuffer[x+y*SCREEN_WIDTH] = color | ((lightbuffer[x+y*SCREEN_WIDTH] & 0xFF) + power);
}


void GFX_init_window() {
    window = SDL_CreateWindow(
        GAME_NAME,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DEFAULT_SCREEN_WIDTH,
        DEFAULT_SCREEN_HEIGHT,
        SDL_WINDOW_FULLSCREEN
    );

    if (window == NULL) {
        printf("window cannot be created");
    }
};

void GFX_init_video() {
    int init_video = SDL_Init(SDL_INIT_VIDEO);

    if(init_video) {
        printf("video cannot be initialized!");
        exit(-1);
    };
};

void GFX_init_renderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    if (renderer == NULL) {
        printf("renderer cannot be initialized!");
    }

};

void GFX_init_png() {
    int imgFlags = IMG_INIT_PNG;

    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
};

void GFX_dealloc_buffers() {
    lightbuffer  = NULL;
    shadowbuffer = NULL;
}

void GFX_clean_buffers() {
    memset(lightbuffer,  BLANK_COLOR, FULL_SCREEN_BYTE_SIZE);
    memset(shadowbuffer, BLANK_COLOR, FULL_SCREEN_BYTE_SIZE);
}

void GFX_alloc_buffers() {
    lightbuffer   = (uint32_t*)malloc(FULL_SCREEN_PIX_SIZE * sizeof(uint32_t));
    shadowbuffer  = (uint32_t*)malloc(FULL_SCREEN_PIX_SIZE * sizeof(uint32_t));
}

// every pixel specific graphic will be stored in this texture. Sprites are rendered in normal
// fashion but pixel-sharp shapes needs to be put in the buffer first.
void GFX_init_screen_buffer_texture() {
    screen_texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );
}

int GFX_init_graphics() {
    GFX_init_video();
    GFX_init_window();
    GFX_init_renderer();
    GFX_init_png();
    GFX_alloc_buffers();
    GFX_init_screen_buffer_texture();

    return 1;
};

void GFX_free_texture(
    texture_t *texture
) {
    SDL_DestroyTexture(texture->surface);
    free(texture);
};

void GFX_free() {
    GFX_dealloc_buffers();
    if (window) {
        SDL_DestroyWindow(window);
    }

    if (renderer) {
        // this gives error
        // SDL_DestroyRenderer(renderer);
        free(renderer);
    }

    free(screen_texture);
    IMG_Quit();

};

void GFX_clear_screen() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
};

void GFX_update() {
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
};

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

// renders texture to screen
void GFX_render_tile(
    texture_t *texture,
    int        render_x,
    int        render_y,
    int        x,
    int        y,
    int        w,        
    int        h
) {
    SDL_Rect clip = {x, y, w, h};
    SDL_Rect render_quad = {render_x, render_y, w, h};
    SDL_RenderCopy(renderer, texture->surface, &clip, &render_quad);
};

// renders texture to screen
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

// simplest line - only for debugging
void GFX_draw_colored_line(
    int x1,
    int y1,
    int x2,
    int y2,
    int r,
    int g,
    int b,
    int a
) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};

// simplest rect - only for debugging
void GFX_draw_rect_border(
    int x,
    int y,
    int w,
    int h,
    int r,
    int g,
    int b,
    int a
) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_Rect rect = (SDL_Rect){x, y, w, h};
    SDL_RenderDrawRect(renderer, &rect);
};

// fills rect with pix_fill_fun function
void GFX_fill_rect(
    void  (*pix_fill_fun)(uint32_t, int, int, int),
    int x,
    int y,
    int w,
    int h,
    int r,
    int g,
    int b,
    int power 
) {
    uint32_t red_color = r << 24;
    uint32_t green_color = g << 16;
    uint32_t blue_color = b << 8;
    uint32_t light_color = red_color | green_color | blue_color;

    for (int xx=x; xx<x+w; xx++) { 
        for(int yy=y; yy<y+h; yy++) {
            pix_fill_fun(light_color, xx, yy, power);
        }
    }
};

void GFX_draw_line_to_buffer(
    int     x1,                                       // starting point
    int     x2,                                       // end point
    void  (*pix_fill_fun)(uint32_t, int, int, int),   // pix fill function
    int     y,                                        // y-axis coef
    int     r,                                        // red color (from 00 to FF)
    int     g,                                        // green color (from 00 to FF)
    int     b,                                        // blue color (from 00 to FF)
    int     power                                     // power of  color (mostly alpha channel)
) {

    uint32_t red_color = r << 24;
    uint32_t green_color = g << 16;
    uint32_t blue_color = b << 8;
    uint32_t light_color = red_color | green_color | blue_color;

    for (int x=x1; x<x2; x++) {
        pix_fill_fun(light_color, x, y, power);
    }
};

sorted_list_t* GFX_calc_intersections_in_scanline(
    segment_t *segments,
    int        y,
    int       *n
) {
    sorted_list_t *intersections = NULL;
    segment_t        *ptr           = NULL;
    int               x;

    ptr = segments;

    while(ptr){
        // line in perpendicular to y-axis
        if (ptr->y1 == ptr->y2){
            ptr=ptr->next;
        }
        // line in perpendicular to x-axis
        else if (ptr->x1 == ptr->x2) {
            SRTLST_insert(&intersections, ptr->x1);
            ptr=ptr->next;
            (*n)++;
        }
        else {
            x = GEO_x_intersection_with_slope(y, ptr->x1, ptr->y1, ptr->slope);
            SRTLST_insert(&intersections, x);
            ptr=ptr->next;
            (*n)++;
        }
    }

    return intersections;
}

void GFX_draw_scanline(
    segment_t     *segments,
    void         (*pix_fill_fun)(uint32_t, int, int, int),
    int            y,
    int            r,
    int            g,
    int            b,
    int            power 
) {
    int            n             = 0;
    sorted_list_t *intscs = NULL;
    intscs                = GFX_calc_intersections_in_scanline(segments, y, &n);

    if (intscs) {

        sorted_list_t* ptr = NULL;

        if (n==2) {
            GFX_draw_line_to_buffer(intscs->value, intscs->next->value, pix_fill_fun, y, r, g, b, power);
        }

        else if (n>2) {
            ptr = intscs;

            while (ptr->next) {
                GFX_draw_line_to_buffer(ptr->value, ptr->next->value, pix_fill_fun, y, r, g, b, power);

                ptr=ptr->next;
                if (ptr->next == NULL) {
                    break;
                }
                ptr=ptr->next;
            }
        }

        SRTLST_free(intscs);
    }
}

// function firstly checks where the polygon "begins" (vertex with highest y coord value) and draw
// dark rect from y=0 up to y of such vertex. Then iterating by every y up to "end" of polygon
// (vertex with lowest y coord value), polygon segments which will be drawn is chosen (and these
// ones which won`t be needed are discarded), intersection points are calculated and then "dark and
// "light" sectors are filled.
void GFX_fill_buffer_single_polygon(
    vertex_t     *poly,
    void          (*pix_fill_fun)(uint32_t, int, int, int),
    int           r,
    int           g,
    int           b,
    int           power
) {
    int         y            = VRTX_highest_y(poly);
    segment_t *not_drawn_yet = SEG_get_segments_of_polygon(poly); 
    segment_t *current_draw  = NULL;
    segment_t *obstacle_ptr  = NULL;
    segment_t *candidates    = NULL;

    while(y<SCREEN_HEIGHT) {
        obstacle_ptr = current_draw;

        // delete segments from current drawn scan_y must be higher than y of any point
        while(obstacle_ptr) {
            SEG_delete(&current_draw, y);
            obstacle_ptr=obstacle_ptr->next;
        }

        // get candidates to draw
        candidates = SEG_find_candidates(&not_drawn_yet, y);

        // add candidates to current draw
        SEG_merge(&current_draw, candidates);       

        // if there isn`t anything to draw or anything to be drawn in future - stop
        if (!not_drawn_yet && !current_draw) { break; }

        GFX_draw_scanline(current_draw, pix_fill_fun, y, r, g, b, power);
        y++;
    }

    SEG_free(not_drawn_yet);
    SEG_free(current_draw);
    SEG_free(obstacle_ptr);
    SEG_free(candidates);
}

// sometimes gradient gradient texture is small enough to not fill entire screen leaving some "gaps"
// of unlighted area. This function ensures that these gaps are left in darkness.
void GFX_fill_gradient_gaps(
    uint32_t     *light_power_buffer,
    texture_t    *gradient_texture,
    int           st_x,
    int           st_y
) {
    int top    = st_y - (int) ((gradient_texture->height) / 2);
    int bottom = top + gradient_texture->height;
    int left   = st_x - (int) ((gradient_texture->width) / 2);
    int right  = left + gradient_texture->width;
    
    if (top < 0) { top = 0; };
    if (bottom < SCREEN_HEIGHT) { bottom = SCREEN_HEIGHT; };
    if (left < 0) { left = 0; };
    if (right > SCREEN_WIDTH) { right = SCREEN_WIDTH; };

    // UPPER PART
    for(int y=0; y<top; y++) {
        for (int x=0; x<SCREEN_WIDTH; x++) {
            light_power_buffer[x+y*SCREEN_WIDTH] = BLANK_COLOR;
        }
    }

    // MIDDLE PART
    for(int y=top; y<bottom; y++) {
        for (int x=0; x<left; x++) {
            light_power_buffer[x+y*SCREEN_WIDTH] = BLANK_COLOR;
        }

        for (int x=right; x<SCREEN_WIDTH; x++) {
            light_power_buffer[x+y*SCREEN_WIDTH] = BLANK_COLOR;
        }
    }

    // BOTTOM PART
    for(int y=bottom; y<SCREEN_HEIGHT; y++) {
        for (int x=0; x<SCREEN_WIDTH; x++) {
            light_power_buffer[x+y*SCREEN_WIDTH] = BLANK_COLOR;
        }
    }
}

// Calculates rectangle shaped as gradient texture (to fill its negation)
SDL_Rect GFX_calc_gradient_rect(
    texture_t* gradient,
    int x,
    int y
) {
    SDL_Rect gradient_quad = {
        x - (int)((gradient->width)/2),
        y - (int)((gradient->height)/2),
        gradient->width,
        gradient->height
    };

    return gradient_quad;
}

// Draws whole light effect by sequientally blitting lightbuffers (in a different manner) to screen
// surface.
void GFX_draw_lightbuffer(
    texture_t* gradient,
    int x,
    int y
) {
    // LIGHT
    SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(screen_texture, NULL, lightbuffer, PIX_PER_SCREEN_ROW);
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
    
    // LIGHT GRADIENT
    SDL_SetTextureBlendMode(gradient->surface, SDL_BLENDMODE_MOD);
    SDL_Rect gradient_quad = GFX_calc_gradient_rect(gradient, x, y);
    SDL_RenderCopy(renderer, gradient->surface, NULL, &gradient_quad);

    // DARKNESS
    GFX_fill_gradient_gaps(lightbuffer, gradient, x, y);
    SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_MOD);
    SDL_UpdateTexture(screen_texture, NULL, lightbuffer, PIX_PER_SCREEN_ROW);
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

    // LIGHT PENETRATING WALLS
    SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(screen_texture, NULL, shadowbuffer, PIX_PER_SCREEN_ROW);
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
}
