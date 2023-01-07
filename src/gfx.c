#include "assets.h"
#include "global.h"
#include "gfx.h"
#include "primitives.h"
#include "sorted_list.h"
#include "geometry.h"
#include "vertex.h"
#include "segment.h"


SDL_Window   *window            = NULL;
SDL_Renderer *renderer          = NULL;
SDL_Texture  *screen_texture    = NULL;

texture_t    *sprites[ASSET_SPRITE_ALL];

uint32_t     *lightbuffer;
uint32_t     *shadowbuffer;

float GFX_dist(
    int x0,
    int y0,
    int x1,
    int y1
) {
    return sqrt((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1)) * 1.5;
}

// this is OpenGL version of mix function
float GFX_lerp(
    float v0,
    float v1,
    float t
) {
    return (1 - t) * v0 + t * v1;
}

// this is the only way to use OpenGL without really calling it. OpenGL implementaiton must be done
// in separate ticket
// proper gradient version for GLSL:
// float dist(vec2 p0, vec2 pf) {return sqrt((pf.x-p0.x)*(pf.x-p0.x)+(pf.y-p0.y)*(pf.y-p0.y));}
//
// void mainImage( out vec4 fragColor, in vec2 fragCoord )
// {
//  float d = dist(iResolution.xy*0.5,fragCoord.xy)*0.01;
// 	fragColor = mix(vec4(1.0, 1.0, 1.0, 1.0), vec4(0.0, 0.0, 0.0, 1.0), d);
// }
//
void GFX_fill_lightbuffer(
    uint32_t  color,
    int       x,
    int       y,
    int       power,
    int       x0,
    int       y0
) {
    // TODO (LG-10): this should be a three (?) separate shaders in OpenGL!
    int      shadow = 0;
    int      pos    = x+y*SCREEN_WIDTH;
    uint32_t dist   = (uint32_t)GFX_dist(x0, y0, x, y);

    if (dist < 255) {
        // fill light color, only if light reaches other light
        shadow = (int)(GFX_lerp(255, 0, (float)MIN(dist, 255)/255));
    }

    lightbuffer[pos] = color | ((lightbuffer[pos] & 0xFF) + power);

    // add shadow with light gradient
    shadowbuffer[pos] = (shadow << 24) | (shadow << 16) | (shadow << 8) | 255; 
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
    memset(lightbuffer, BLANK_COLOR, FULL_SCREEN_BYTE_SIZE);
    memset(shadowbuffer, BLANK_COLOR, FULL_SCREEN_BYTE_SIZE);
}

void GFX_alloc_buffers() {
    lightbuffer     = NULL;
    shadowbuffer = NULL;

    lightbuffer     = (uint32_t*)malloc(FULL_SCREEN_PIX_SIZE * sizeof(uint32_t));
    shadowbuffer = (uint32_t*)malloc(FULL_SCREEN_PIX_SIZE * sizeof(uint32_t));
}

void GFX_free_buffers() {
    free(lightbuffer);
    free(shadowbuffer);

    lightbuffer     = NULL;
    shadowbuffer = NULL;
}

// every pixel specific graphic will be stored in this texture. Sprites are rendered in normal
// fashion but pixel-sharp shapes needs to be put in the buffer first.
void GFX_init_screen_buffer_texture(
) {
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
    texture = NULL;
};

void GFX_free() {
    GFX_free_buffers();
    SDL_DestroyTexture(screen_texture);

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }

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
    // SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
};

// fills rect with shader function
void GFX_fill_rect(
    shader_t shader,
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
            shader(light_color, xx, yy, power, 0, 0);
        }
    }
};

void GFX_draw_line_to_buffer(
    int       x1,                                       // starting point
    int       x2,                                       // end point
    shader_t  shader,
    int       y,                                        // y-axis coef
    int       r,                                        // red color (from 00 to FF)
    int       g,                                        // green color (from 00 to FF)
    int       b,                                        // blue color (from 00 to FF)
    int       power,                                    // power of  color (mostly alpha channel)
    int       x0,
    int       y0
) {

    uint32_t red_color = r << 24;
    uint32_t green_color = g << 16;
    uint32_t blue_color = b << 8;
    uint32_t light_color = red_color | green_color | blue_color;

    for (int x=MAX(x1, 0); x<MIN(SCREEN_WIDTH, x2); x++) {
        shader(light_color, x, y, power, x0, y0);
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
            SRTLST_insert(&intersections, ptr->x1,0 );
            ptr=ptr->next;
            (*n)++;
        }
        else {
            x = GEO_x_intersection_with_slope(y, ptr->x1, ptr->y1, ptr->slope);
            SRTLST_insert(&intersections, x, 0);
            ptr=ptr->next;
            (*n)++;
        }
    }

    return intersections;
}

void GFX_draw_scanline(
    segment_t     *segments,
    shader_t       shader,
    int            y,
    int            r,
    int            g,
    int            b,
    int            power,
    int            x0,
    int            y0
) {
    int            n      = 0;
    sorted_list_t *intscs = NULL;
    intscs                = GFX_calc_intersections_in_scanline(segments, y, &n);

    if (intscs) {

        sorted_list_t* ptr = NULL;

        if (n==2) {
            GFX_draw_line_to_buffer(intscs->value, intscs->next->value, shader, y, r, g, b, power, x0, y0);
        }

        else if (n>2) {
            ptr = intscs;

            while (ptr->next) {
                GFX_draw_line_to_buffer(ptr->value, ptr->next->value, shader, y, r, g, b, power, x0, y0);

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

// Fill texture (lightbuffer) with polygon (being geometric shape where light is present and should
// be drawn). Polygon is expressed as linked list of vertices, scanline algorithm is used to
// transpose those vertices to filled polygon.
// Function is secured from drawing outside the screen, as such behavior is unsave and wastes
// resources.
void GFX_fill_light(
    shader_t shader,
    vertex_t     *poly,
    int           r,
    int           g,
    int           b,
    int           power,
    int           x0,
    int           y0
) {
    int         y            = 0;
    int         highest      = VRTX_highest_y(poly);
    int         lowest       = VRTX_lowest_y(poly);
    
    segment_t *not_drawn_yet = SEG_get_segments_of_polygon(poly); 
    segment_t *current_draw  = NULL;
    segment_t *obstacle_ptr  = NULL;
    segment_t *candidates    = NULL;

    // algorithm is starting from highest vertex of polygon...
    y = highest;

    // up to the lowest vertex of polygon
    while(y<lowest) {

        // if algorithm reaches point below screen, no further drawing is needed, algorithm is
        // terminated
        if (y>SCREEN_WIDTH) { y++; break; }

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

        // if current scanline y is below 0 (outside the screen, no drawing is needed, go to next
        // scanline)
        if (y<0) { y++; continue; }
        else { 
            GFX_draw_scanline(current_draw, shader, y, r, g, b, power, x0, y0);
            y++;
        }
    }

    if (not_drawn_yet) { SEG_free(not_drawn_yet); }
    if (current_draw) { SEG_free(current_draw); }
    if (obstacle_ptr) { SEG_free(obstacle_ptr); }
    if (candidates)  { SEG_free(candidates); }
}

texture_t* GFX_read_texture(
    const char *filepath
) {
	SDL_Texture *new_texture    = NULL;
    SDL_Surface *loaded_surface = NULL;

    loaded_surface              = IMG_Load(filepath);

    // dummy texture if reading file failed
    if (loaded_surface == NULL) {
        new_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 20, 20);
        texture_t* p = malloc(sizeof(texture_t));

        p->surface = new_texture;
        p->width   = loaded_surface->w;
        p->height  = loaded_surface->h;

        return p;
    }
    else {
        SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x80, 0xFF, 0xFF));
        new_texture  = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        texture_t* p = malloc(sizeof(texture_t));

        p->surface = new_texture;
        p->width   = loaded_surface->w;
        p->height  = loaded_surface->h;

        SDL_FreeSurface(loaded_surface);

        return p;
    }
};

void GFX_draw_light(
) {
    // LIGHT
    SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_BLEND);
    SDL_UpdateTexture(screen_texture, NULL, lightbuffer, PIX_PER_SCREEN_ROW);
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);

}

void GFX_draw_darkness(
) {
    // DARKNESS
    SDL_SetTextureBlendMode(screen_texture, SDL_BLENDMODE_MOD);
    SDL_UpdateTexture(screen_texture, NULL, shadowbuffer, PIX_PER_SCREEN_ROW);
    SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
}
