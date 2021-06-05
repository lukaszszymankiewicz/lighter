#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "draw.h"

Texture* hero_sprites;
SDL_Rect hero_texture_clips[ 4 ];

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;

// SETUP
SDL_Window* create_window(int width, int height) {
    window = SDL_CreateWindow(
       "Lighter",
       SDL_WINDOWPOS_UNDEFINED,
       SDL_WINDOWPOS_UNDEFINED,
       width,
       height,
       SDL_WINDOW_SHOWN
    );
    if (window == NULL) {
        printf("window cannot be created");
    }
    return window;
};

void init_video() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("video canno be initialized!");
    };
};

SDL_Renderer* init_renderer(SDL_Window* window) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        printf("renderer cannot be initialized!");
    }
    return renderer;
};

void init_png() {
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
};

int init_graphics(int width, int height) {
    init_video();
    window = create_window(width, height);
    renderer = init_renderer(window);
    init_png();
    enable_alpha_bleeding();

    return 1;
};

Texture *init_texture(char *filepath) {
	SDL_Texture* new_texture = NULL;
    SDL_Surface* loaded_surface = NULL;

    loaded_surface = IMG_Load(filepath);

    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0x80, 0xFF, 0xFF));

    new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

    Texture *p = malloc(sizeof(Texture));

    p->surface = new_texture;
    p->width = loaded_surface->w;
    p->height = loaded_surface->h;

    SDL_FreeSurface(loaded_surface);

    return p;
};

void load_frames() {
    hero_sprites = init_texture("sprites/hero.png");

    hero_texture_clips[ 0 ].x =  0;
    hero_texture_clips[ 0 ].y =  0;
    hero_texture_clips[ 0 ].w = 30;
    hero_texture_clips[ 0 ].h = 45;

    //Set top right sprite
    hero_texture_clips[ 1 ].x = 31;
    hero_texture_clips[ 1 ].y =  0;
    hero_texture_clips[ 1 ].w = 30;
    hero_texture_clips[ 1 ].h = 45;
    
    //Set bottom left sprite
    hero_texture_clips[ 2 ].x = 62;
    hero_texture_clips[ 3 ].y =  0;
    hero_texture_clips[ 2 ].w = 30;
    hero_texture_clips[ 2 ].h = 45;

    //Set bottom right sprite
    hero_texture_clips[ 3 ].x = 93;
    hero_texture_clips[ 3 ].y =  0;
    hero_texture_clips[ 3 ].w = 30;
    hero_texture_clips[ 3 ].h = 45;
};

void enable_alpha_bleeding() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
};

void free_texture(Texture *texture) {
    SDL_DestroyTexture(texture->surface);
    free(texture);
};

void LRE_free_graphics() {
    free_texture(hero_sprites);

    window = NULL;
    renderer = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    SDL_Quit();
};

// DRAW
void LRE_draw_hero_sprite(int x, int y, int hero_state) {
    render_texture(hero_sprites, &hero_texture_clips[hero_state], x, y);
};

void render_texture(Texture* texture, SDL_Rect* clip, int x, int y) {
    SDL_Rect render_quad = {x, y, texture->width, texture->height};

    if(clip != NULL) {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopy(renderer, texture->surface, clip, &render_quad);
};

void LRE_clear_screen() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
};

void LRE_update_graphics() {
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
};

float find_intersection_coef(
    float ray_x1, float ray_y1,
    float ray_x2, float ray_y2,
    float wall_x1, float wall_y1,
    float wall_x2, float wall_y2
)
{
	float ray_dx = ray_x2 - ray_x1;
	float ray_dy = ray_y2 - ray_y1;
	float wall_dx = wall_x2 - wall_x1;
	float wall_dy = wall_y2 - wall_y1;

	float r_mag = sqrt(ray_dx * ray_dx + ray_dy * ray_dy);
	float s_mag = sqrt(wall_dx * wall_dx + wall_dy * wall_dy);

	if(ray_dx/r_mag==wall_dx/s_mag && ray_dy/r_mag==wall_dy/s_mag) {return -1;}

	float T2 = (ray_dx*(wall_y1-ray_y1) + ray_dy*(ray_x1-wall_x1))/(wall_dx*ray_dy - wall_dy*ray_dx);
	float T1 = (wall_x1+wall_dx*T2-ray_x1)/ray_dx;

	if (T1<0 || T2<0 || T2>1) {return -1;}

    return T1;
};

int is_better_coef(float a, float b) {
    if (a == -1 && b != -1) {return 1;}
    else if (a!=-1 && b!=-1 && a>b) {return 1;}
    else {return 0;}
};


void LRE_draw_colored_line(float x1, float y1, float x2, float y2, int r, int g, int b, int a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};


// atan2 function from math is used to calculate angle, more naive and faster approach can be taken
// as this is rather slow function. Please note that not direct angle is used in here as it is only
// for sorting purposes - some more naive implementation coudl be easily used.
float angle(int ax, int ay, int bx, int by) {
    return atan2(ax - bx, ay - by);
}

// Function to insert a given node at its correct sorted position into a given
// list sorted in increasing order
void insert_LightPoint(struct LightPoint** head, struct LightPoint* new_point) {
    struct LightPoint** currentRef = head;
    while (*currentRef != NULL && (*currentRef)->angle < new_point->angle) {
        currentRef = &((*currentRef)->next);
    }
 
    new_point->next = *currentRef;
    *currentRef = new_point;
}
 
// Helper function to return a new node of the linked list
struct LightPoint* new_Point(int x, int y, int st_x, int st_y) {
    struct LightPoint* new_point = (struct LightPoint*)malloc(sizeof(struct LightPoint));
    new_point->x = x;
    new_point->y = y;
    new_point->angle = angle(x, y, st_x, st_y);
    new_point->next = NULL;

    return new_point;
}

// for each corner of wall three ays are drawn (one directly into corner, one a little bit to left, 
// and one a little bit to right). This allows to calculate map pieces where there is high lightness
// and where there is only a little bit of it (rays which are a little bit offseted will mark it).
void LRE_draw_rays(Position hero_pos, PolyLine * walls) { 
    
    int correction[] = {0, -5, 5};
    int corrections = 3;

    // Start of the light polygon to be drawn
    struct LightPoint* light_poly = NULL;

    for(int c=0; c<corrections; c++) {
        int corr = correction[c];
        PolyLine * cur_wall = NULL;
        cur_wall = walls;

        for(; cur_wall; cur_wall=cur_wall->next) {
            float best_x = cur_wall->x1;
            float best_y = cur_wall->y1;

            // "base" intersection - not corrected rays always hits it target, corrected rays can
            // overshoot - but because level is always an closed polygon, ray always hits smth but
            // only is to find what
            float intersection_coef = find_intersection_coef(
                hero_pos.x, hero_pos.y,
                cur_wall->x1, cur_wall->y1 + corr,    
                cur_wall->x1, cur_wall->y1,    
                cur_wall->x2, cur_wall->y2    
            );
            
            PolyLine * hit_wall = NULL;
            hit_wall = walls;

            for(; hit_wall; hit_wall=hit_wall->next) {
                float new_intersection_coef = find_intersection_coef(
                    hero_pos.x, hero_pos.y,
                    cur_wall->x1, cur_wall->y1 + corr,    
                    hit_wall->x1, hit_wall->y1,    
                    hit_wall->x2, hit_wall->y2    
                );

                // closer intersection is found. x and y coef for ray is stored.
                if (is_better_coef(intersection_coef, new_intersection_coef)) {
                    intersection_coef = new_intersection_coef;
                    best_x = hero_pos.x + (cur_wall->x1 - hero_pos.x) * intersection_coef;
                    best_y = hero_pos.y + (cur_wall->y1 + corr - hero_pos.y) * intersection_coef;
                }
            }
            insert_LightPoint(&light_poly, new_Point(best_x, best_y, hero_pos.x, hero_pos.y));
        }
    }

    struct LightPoint* ptr = light_poly;

    int last_x = ptr->x; // we will save it for later on
    int last_y = ptr->y; // we will save it for later on

    // iterating by light polygon points and drawing triangle by triangle
    while (ptr) {
        int first_x = ptr->x; 
        int first_y = ptr->y; 
        ptr = ptr->next;

        if (ptr==NULL) {
            LRE_fill_triangle(first_x, first_y, last_x, last_y, hero_pos.x, hero_pos.y, 49, 50, 50, 255);
            break;
        }
        int second_x = ptr->x; 
        int second_y = ptr->y; 
        LRE_fill_triangle(first_x, first_y, second_x, second_y, hero_pos.x, hero_pos.y, 50, 50, 50, 255);
    }
    
    // debug lines, comments if not needed
    // ptr = light_poly;
    // while (ptr)
    // {
    //     LRE_draw_colored_line(hero_pos.x, hero_pos.y, ptr->x, ptr->y, 255, 255, 255, 255);
    //     ptr = ptr->next;
    // }


    free_PolyLine(light_poly);
};


// point are sorted by y increasingly, we could use full more sofisticated sorting, but here it is
// so basic that this task handled easily by f-conditions.  points should be sorted in a way that y1
// < y2 < y3 (x-coords are not taken into sorting).
void LRE_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b, int a){
    int swapx;
    int swapy;
    
    if (y1 > y3) {swapx=x3; swapy=y3; x3=x1; y3=y1; x1=swapx; y1=swapy;}
    if (y2 > y3) {swapx=x3; swapy=y3; x3=x2; y3=y2; x2=swapx; y2=swapy;}
    if (y1 > y2) {swapx=x2, swapy=y2; x2=x1; y2=y1; x1=swapx; y1=swapy;}
    
    for (int y=y1; y<y2; y++) {
        float linex1 = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
        float linex2 = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        LRE_draw_colored_line(linex1, y, linex2, y, r, g, b, a);
    }
    for (int y=y2; y<y3; y++) {
        float linex1 = x2 + (x3 - x2) * (y - y2) / (y3 - y2);
        float linex2 = x1 + (x3 - x1) * (y - y1) / (y3 - y1);
        LRE_draw_colored_line(linex1, y, linex2, y, r, g, b, a);
    }
};

PolyLine * new_PolyLine(int x1, int y1, int x2, int y2) {
    PolyLine * new_PolyLine = (PolyLine *)malloc(sizeof(PolyLine *));
    new_PolyLine->x1 = x1;
    new_PolyLine->y1 = y1;
    new_PolyLine->x2 = x2;
    new_PolyLine->y2 = y2;
    new_PolyLine->next = NULL;

    return new_PolyLine;
}

void insert_PolyLine(PolyLine** head, PolyLine* new_PolyLine) {
    PolyLine** currentRef = head;

    while (*currentRef != NULL) {
        currentRef = &((*currentRef)->next);
    }

    new_PolyLine->next = *currentRef;
    *currentRef = new_PolyLine;
}

void free_PolyLine(struct LightPoint* head) {
    struct LightPoint* currentRef = head;

    while (currentRef != NULL) {
        struct LightPoint * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}
