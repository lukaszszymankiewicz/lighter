#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "draw.h"

#define DEBUG 0

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

struct LightPoint* new_Point(
    int x,        int y,
    int st_x,     int st_y,
    int wall_id
)
{
    struct LightPoint* new_point = (struct LightPoint*)malloc(sizeof(struct LightPoint));
    new_point->x = x;
    new_point->y = y;
    new_point->wall_id = wall_id;
    new_point->angle = angle(x, y, st_x, st_y);
    new_point->next = NULL;

    return new_point;
}

void insert_LightPoint(
    struct LightPoint** head,
    int x1, int y1,
    int x2, int y2,
    int wall_id
) 
{

    struct LightPoint* current;
    struct LightPoint* new_pt = new_Point(x1, y1, x2, y2, wall_id);

    if (*head == NULL || (*head)->angle >= new_pt->angle)
    {
        new_pt->next = *head;
        *head = new_pt;
    }
    else 
    {
        current = *head;

        while (current->next != NULL && current->next->angle < new_pt->angle) 
        {
            current = current->next;
        }

        new_pt->next = current->next;
        current->next = new_pt;
    }

}
 
int lines_intersects(
    int x1, int y1,     // second line
    int x2, int y2,     // second line
    int x3, int y3,     // second line
    int x4, int y4,     // second line
    int *i_x, int *i_y  // intersection points
)
{
    float s_numer, t_numer, denom, t;
    float dx13, dy13;
    float dx21, dx43, dy21, dy43;

    dx21 = x2 - x1; dy21 = y2 - y1; dx43 = x4 - x3; dy43 = y4 - y3;
    denom = dx21 * dy43 - dx43 * dy21;

    if (denom == 0)
        return 0; // Collinear

    bool denomPositive = denom > 0;

    dx13 = x1 - x3; dy13 = y1 - y3;
    s_numer = dx21 * dy13 - dy21 * dx13;

    if ((s_numer < 0) == denomPositive)
        return 0; // No collision

    t_numer = dx43 * dy13 - dy43 * dx13;
    if ((t_numer < 0) == denomPositive)
        return 0; // No collision

    if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
        return 0; // No collision

    // Collision detected
    t = t_numer / denom;
    if (i_x != NULL)
        *i_x = (int)x1 + (t * dx21);
    if (i_y != NULL)
        *i_y = (int)y1 + (t * dy21);

    return 1;
}

// rays are always cast from hero standing point so ray_x1 and ray_y1 wil be equal to hero pos
// ray_x2 and ray_y2 are corrds of current polygon point (for main ray)
// ray_x2 and ray_y2 are corrds offseted points behind the level (for aux rays), offeseted points
// are calculated from polar coordinates (we take big radius and calculate appripriate angle)
void LRE_find_shortest_ray(
    int ray_x1, int ray_y1,
    int ray_x2, int ray_y2,
    int * best_x, int * best_y,
    PolyLine * walls, int * wall_id
)
{ 
        int shortest_x = ray_x2;
        int shortest_y = ray_y2;
        int cur_x = shortest_x; 
        int cur_y = shortest_y;
        int id = 0; // if resulted best ray will have wall id equal to zero it means that no intersection occures

        PolyLine * hit_wall = NULL;
        hit_wall = walls;

        for(; hit_wall; hit_wall=hit_wall->next) 
        {
            id++;
            if (lines_intersects(
                ray_x1,       ray_y1,       shortest_x,   shortest_y,    
                hit_wall->x1, hit_wall->y1, hit_wall->x2, hit_wall->y2,
                &cur_x,       &cur_y                                    )
            )
            {
                if (abs(ray_x1 - cur_x) < abs(ray_x1 - shortest_x))
                {
                    shortest_x = cur_x; shortest_y = cur_y; *wall_id = id;
                }
            }
        }

    *best_x = (int)shortest_x;
    *best_y = (int)shortest_y;
}
// Light polygon has many points and a number of it is redundant and can be deleted withaout any
// heasitation. this function does exacly this
void LRE_optim_LightPoly(struct LightPoint* poly)
{
    while (poly->next != NULL && poly->next->next != NULL)
    {
        struct LightPoint* cur_cone = poly->next;

        if (cur_cone->next->wall_id == poly->wall_id && cur_cone->next->wall_id == cur_cone->wall_id)
        {
            // there is same wall id behing current cone and after current cone - so current
            // cone can be deleted.
            //
            //             current
            //               | 
            //               v
            //    +------+-------+-------+
            //    |  5   |   5   |   5   |  ... 
            //    +------+-------+-------+
            poly->next = cur_cone->next;            
            free(cur_cone);
        }
        else 
        {
            // there isnt same wall id behing current cone and after current cone - we`re moving on
            //
            //             current
            //               | 
            //               v
            //    +------+-------+-------+
            //    |  5   |   5   |   3   |  ... 
            //    +------+-------+-------+
            //
            //               OR
            //
            //             current
            //               | 
            //               v
            //    +------+-------+-------+
            //    |  3   |   5   |   5   |  ... 
            //    +------+-------+-------+
            poly=poly->next;
        }
    }
}

// for each corner of wall three ays are drawn (one directly into corner, one a little bit to left, 
// and one a little bit to right). This allows to calculate map pieces where there is high lightness
// and where there is only a little bit of it (rays which are a little bit offseted will mark it).
void LRE_draw_rays(Position hero_pos, PolyLine * walls) { 

    struct LightPoint* light_poly = NULL;
    PolyLine * cur_wall = NULL;
    cur_wall = walls;
    int wall_id = 0; // this will monitor current wall id to optimize light triangle

    for(; cur_wall; cur_wall=cur_wall->next) {
        // "base" intersection - not corrected rays always hits it target, corrected rays can
        // overshoot - but because level is always an closed polygon, ray always hits smth but
        // only is to find what
        float pl_angle = angle(hero_pos.x, hero_pos.y, cur_wall->x1, cur_wall->y1);
        float corr = 0.01;
        int big_r = 1000;
        int shortest_x, shortest_y;

        // main ray
        LRE_find_shortest_ray(
            hero_pos.x,                                hero_pos.y,
            cur_wall->x1,                              cur_wall->y1,
            &shortest_x,                               &shortest_y,
            walls,                                     &wall_id
        );
        insert_LightPoint(&light_poly, shortest_x, shortest_y, hero_pos.x, hero_pos.y, wall_id);

        // shifted ray
        LRE_find_shortest_ray(
            hero_pos.x,                                hero_pos.y,
            hero_pos.x - sin(pl_angle + corr) * big_r, hero_pos.y - cos(pl_angle + corr) * big_r,
            &shortest_x,                               &shortest_y,
            walls,                                     &wall_id
        );
        insert_LightPoint(&light_poly, shortest_x, shortest_y, hero_pos.x, hero_pos.y, wall_id);

        // shifted ray
        LRE_find_shortest_ray(
            hero_pos.x,                                hero_pos.y,
            hero_pos.x - sin(pl_angle - corr) * big_r, hero_pos.y - cos(pl_angle - corr) * big_r,
            &shortest_x,                               &shortest_y,
            walls,                                     &wall_id
        );
        insert_LightPoint(&light_poly, shortest_x, shortest_y, hero_pos.x, hero_pos.y, wall_id);

    }

    struct LightPoint* ptr = light_poly;

    int last_x = ptr->x; // we will save it for later on
    int last_y = ptr->y; // we will save it for later on

    // iterating by light polygon points and drawing triangle by triangle
    int len = 0;

    while (ptr) {
        len++;
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
    LRE_optim_LightPoly(light_poly);

    if (DEBUG)
    {
        ptr = light_poly;
        while (ptr)
        {
            LRE_draw_colored_line(hero_pos.x, hero_pos.y, ptr->x, ptr->y, 255, 255, 255, 255);
            ptr = ptr->next;
        }
        free_PolyLine(ptr);
    }
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
