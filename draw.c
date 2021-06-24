#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "draw.h"

#define DEBUG 1
#define MAX(a,b) ((a) > (b) ? (a) : (b))  // yeah, good old ANSI C tricks here
#define MIN(a,b) ((a) < (b) ? (a) : (b))

Texture* hero_sprites;
Texture* light_gradient;
SDL_Rect hero_texture_clips[ 4 ];
SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;

// SETUP
SDL_Window* create_window(int w, int h) 
{
    window = SDL_CreateWindow("Lighter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);
    if (window == NULL) 
    {
        printf("window cannot be created");
    }
    return window;
};

void init_video() 
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("video canno be initialized!");
    };
};

SDL_Renderer* init_renderer(SDL_Window* window) 
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) 
    {
        printf("renderer cannot be initialized!");
    }
    return renderer;
};

void init_png() 
{
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags)) 
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }
};

int init_graphics(int width, int height) 
{
    init_video();
    window = create_window(width, height);
    renderer = init_renderer(window);
    init_png();
    enable_alpha_bleeding();

    return 1;
};

Texture *init_texture(char *filepath) 
{
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

void load_frames() 
{
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

    // light gradient
    light_gradient = init_texture("sprites/gradient.png");
};

void enable_alpha_bleeding() 
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
};

void free_texture(Texture *texture) 
{
    SDL_DestroyTexture(texture->surface);
    free(texture);
};

void LRE_free_graphics() 
{
    free_texture(hero_sprites);

    window = NULL;
    renderer = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    IMG_Quit();
    SDL_Quit();
};

// DRAW
void LRE_draw_hero_sprite(int x, int y, int hero_state) 
{
    render_texture(hero_sprites, &hero_texture_clips[hero_state], x, y);
};

void render_texture(Texture* texture, SDL_Rect* clip, int x, int y) 
{
    SDL_Rect render_quad = {x, y, texture->width, texture->height};

    if(clip != NULL) 
    {
        render_quad.w = clip->w;
        render_quad.h = clip->h;
    }

    SDL_RenderCopy(renderer, texture->surface, clip, &render_quad);
};

void LRE_clear_screen() 
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
};

void LRE_update_graphics() 
{
    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
};

// PRIMITIVES
void LRE_draw_colored_line(float x1, float y1, float x2, float y2, int r, int g, int b, int a) 
{
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};

// only for DEBUG
void LRE_draw_bg() 
{
    SDL_Rect fillRect = {0, 0, 512, 512};
    SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);     
    SDL_RenderFillRect(renderer, &fillRect);
};

void LRE_draw_ligh_gradient(int x, int y) 
{
    render_texture(light_gradient, NULL, x, y);
};

// atan2 function from math is used to calculate angle, more naive and faster approach can be taken
// as this is rather slow function. Please note that not direct angle is used in here as it is only
// for sorting purposes - some more naive implementation coudl be easily used.
float angle(int ax, int ay, int bx, int by) 
{
    return atan2(ax - bx, ay - by);
}

// Function to insert a given node at its correct sorted position into a given
// list sorted in increasing order
lightpoint_t* new_lightpoint(
    int x,        int y,
    int st_x,     int st_y,
    int wall_id
)
{
    lightpoint_t* new_point = (lightpoint_t*)malloc(sizeof(lightpoint_t));
    new_point->x = x;
    new_point->y = y;
    new_point->wall_id = wall_id;
    new_point->angle = angle(x, y, st_x, st_y);
    new_point->next = NULL;

    return new_point;
}

x_intersection_t* new_x_intersection(
    int x
)
{
    x_intersection_t* new_x_intersection = (x_intersection_t*)malloc(sizeof(x_intersection_t));
    new_x_intersection->x = x;
    return new_x_intersection;
}

void insert_x_intersection(
    x_intersection_t** head, int x 
) 
{
    x_intersection_t* current;
    x_intersection_t* new_intersection = new_x_intersection(x);

    if (*head == NULL)
    {
        new_intersection->next = *head;
        *head = new_intersection;
    }
    // place new point at begininng
    else if ((*head)->x >= new_intersection->x)
    {
        new_intersection->next = *head;
        *head = new_intersection;
    }
    else 
    {
        current = *head;

        while (current->next != NULL && current->next->x < new_intersection->x) 
        {
            current = current->next;
        }
        new_intersection->next = current->next;
        current->next = new_intersection;
    }
}

int get_last_x_intersection
(
    x_intersection_t* intersections
)
{
    x_intersection_t* ptr = NULL;
    ptr = intersections;

    while(ptr->next) 
    {
        ptr=ptr->next;
    }
    return ptr->x;
}

void insert_lightpoint(
    lightpoint_t** head,
    int x1, int y1,
    int x2, int y2,
    int wall_id
) 
{
    lightpoint_t* current;
    lightpoint_t* new_pt = new_lightpoint(x1, y1, x2, y2, wall_id);

    if (*head == NULL)
    {
        new_pt->next = *head;
        *head = new_pt;
    }
    // place new point at begininng
    else if ((*head)->angle >= new_pt->angle)
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
    int x1,   int y1,     // first line
    int x2,   int y2,     // first line
    int x3,   int y3,     // second line
    int x4,   int y4,     // second line
    int *i_x, int *i_y    // intersection points
)
{
    float s_numer, t_numer, denom, t;
    float dx13, dy13;
    float dx21, dx43, dy21, dy43;

    dx21 = x2 - x1; dy21 = y2 - y1; dx43 = x4 - x3; dy43 = y4 - y3;
    denom = dx21 * dy43 - dx43 * dy21;

    if (denom == 0)
        return 0; // collinear

    bool denomPositive = denom > 0;

    dx13 = x1 - x3; dy13 = y1 - y3;
    s_numer = dx21 * dy13 - dy21 * dx13;

    if ((s_numer < 0) == denomPositive)
        return 0; // no collision

    t_numer = dx43 * dy13 - dy43 * dx13;
    if ((t_numer < 0) == denomPositive)
        return 0; // no collision

    if (((s_numer >= denom) == denomPositive) || ((t_numer >= denom) == denomPositive))
        return 0; // no collision

    // collision detected
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
    int ray_x1,         int ray_y1,
    int ray_x2,         int ray_y2,
    int * best_x,       int * best_y,
    polyline_t * walls, int * wall_id
)
{ 
        polyline_t * hit_wall = NULL;
        hit_wall = walls;

        // For first iteration we assume that base ray end is closest to ay begginig.
        int shortest_x = ray_x2;
        int shortest_y = ray_y2;
        int cur_x = shortest_x; 
        int cur_y = shortest_y;

        // If resulted best ray will have wall id equal to zero it means that no intersection,
        // occures. Please note that such event is very unlikely as four corners of the level are
        // also taken into account when chacking light collision. This value serves mostly as debug
        // value.
        int id = 0; 

        for(; hit_wall; hit_wall=hit_wall->next) 
        {
            id++;
            if (lines_intersects(
                ray_x1,       ray_y1,       shortest_x,   shortest_y,    // ray coords (end, beg)
                hit_wall->x1, hit_wall->y1, hit_wall->x2, hit_wall->y2,  // wall coords (end, beg)
                &cur_x,       &cur_y                                  )  // closest point coors are stored here
            )
            {
                // check if newly found intersection point is better than current one, and by better
                // mean closer to ray begginig. As we found points which is always in one line, full
                // distance equations is redundant - we don`t need to check full disance as diff in
                // x coors is enough.
                if (abs(ray_x1 - cur_x) <= abs(ray_x1 - shortest_x)) 
                {
                    shortest_x = cur_x; shortest_y = cur_y; *wall_id = id; 
                }
            }
        }

    // clip to screen edges
    *best_x = MIN(MAX(0, (int)shortest_x), 512);
    *best_y = MIN(MAX(0, (int)shortest_y), 512);
}

void LRE_optim_lightpoint(lightpoint_t* poly)
{
    lightpoint_t* ptr = NULL;
    ptr = poly;

    // cut all redundant in between
    while (ptr->next != NULL && ptr->next->next != NULL)
    { 
       lightpoint_t* cur_cone = ptr->next;
       if (cur_cone->next->wall_id == ptr->wall_id && cur_cone->next->wall_id == cur_cone->wall_id)
       {
           ptr->next = cur_cone->next; free(cur_cone);
       }
       else 
       { ptr=ptr->next; }
    }
    poly = ptr;
}

// scanline is always parallel to y-axis, so simplified equations can be applied. Thus we know that
// in that point scanline is actually intersect with (x1, y1), (x2, y2) segment. No further checks
// is needed.
float scanline_intersection
(
    int scan_y, 
    int x1,      int y1,
    int x2,      int y2
)
{
    return x1 + (x2 - x1) * (scan_y - y1) / (y2 - y1);
}

int segment_intersetcs_with_scanline
(
    int scan_y, int y1, int y2
)
{
    return ((y1 < scan_y && y2 >= scan_y) || (y2 < scan_y && y1 >= scan_y));
}

void LRE_draw_dark_sectors(lightpoint_t* pts)
{
    float x_inter;

    // Scan every y for seach of colision with light polygon.
    for (int scan_y=0; scan_y<512; scan_y++) 
    {
        x_intersection_t* intersections = NULL;
        lightpoint_t* ptr = pts;
        int n=0;
        int first_x = ptr->x;
        int first_y = ptr->y;

        for(; ptr->next; ptr = ptr->next)
        {
            if (segment_intersetcs_with_scanline(scan_y, ptr->y, ptr->next->y))
            {
                x_inter = scanline_intersection(scan_y, ptr->x, ptr->y, ptr->next->x, ptr->next->y);
                insert_x_intersection(&intersections, x_inter);
                n++;
            }
        }

        // last segment
        if (segment_intersetcs_with_scanline(scan_y, ptr->y, first_y))
        {
            x_inter = scanline_intersection(scan_y, ptr->x, ptr->y, first_x, first_y);
            insert_x_intersection(&intersections, x_inter);
            n++;
        }

        // if no intersection found this scanline should be all black (abort drawing)
        if (n==0)
        { LRE_draw_colored_line(0, scan_y, 512, scan_y, 0, 0, 0, 255); continue;}

        int last_x_intersection = get_last_x_intersection(intersections);

        // dark segment from left side of screen to first intersection with scanline
        if (intersections->x > 1)
        { LRE_draw_colored_line(0, scan_y, intersections->x, scan_y, 0, 0, 0, 255);}

        // dark segment from right side of screen to last intersection with scanline
        if (last_x_intersection < 511) 
        { LRE_draw_colored_line(last_x_intersection, scan_y, 512, scan_y, 0, 0, 0, 255); }

        // rest of dark segments
        if (n>2)
        {
            intersections=intersections->next;

            // ugly, but we ommit using crypic next->next->next
            for (int z=1; z<n-1; z+=2)
            {
                LRE_draw_colored_line(intersections->x, scan_y, intersections->next->x, scan_y, 0, 0, 0, 255); 
                intersections=intersections->next->next;
            }
        }
        free_x_intersection(intersections);
    }

}

void LRE_fill_lightpoly(lightpoint_t* light_poly, int st_x, int st_y)
{
    lightpoint_t* ptr = light_poly;

    int last_x = ptr->x; // we will save it for later on
    int last_y = ptr->y; // we will save it for later on

    // iterating by light polygon points and drawing triangle by triangle
    while (ptr) {
        int first_x = ptr->x; 
        int first_y = ptr->y; 
        ptr = ptr->next;

        if (ptr==NULL) {
            // joining last cone with first patching the whole light poly in one.
            LRE_fill_triangle(first_x, first_y, last_x, last_y, st_x, st_y, 50, 50, 50, 255);
            break;
        }
        int second_x = ptr->x; 
        int second_y = ptr->y; 
        LRE_fill_triangle(first_x, first_y, second_x, second_y, st_x, st_y, 50, 50, 50, 255);
    }
}

void LRE_debug_rays(lightpoint_t* light_poly, int st_x, int st_y)
{
    for(lightpoint_t* ptr = light_poly; ptr; ptr = ptr->next)
    { LRE_draw_colored_line(st_x, st_y, ptr->x, ptr->y, 255, 255, 255, 255); }
}

void LRE_debug_dark_sectors(lightpoint_t* light_poly)
{
    int first_x = light_poly->x;
    int first_y = light_poly->y;
    lightpoint_t* ptr = light_poly;

    for(; ptr->next; ptr = ptr->next)
    { LRE_draw_colored_line(ptr->x, ptr->y, ptr->next->x, ptr->next->y, 255, 255, 255, 255); }
    LRE_draw_colored_line(ptr->x, ptr->y, first_x, first_y, 255, 255, 255, 255);
}

// for each corner of wall three ays are drawn (one directly into corner, one a little bit to left, 
// and one a little bit to right). This allows to calculate map pieces where there is high lightness
// and where there is only a little bit of it (rays which are a little bit offseted will mark it).
void LRE_draw_lightpoly(position_t hero_pos, polyline_t * walls) { 

    // each wall can be hit by maximum two rays (all rays in between are redundant), so array with 
    // n_wall * 2 points are ceated to store results and filled with empty data. After all point are
    // catched (and promoted if needed), list is then sort by its angle (to form a poly).
    lightpoint_t* light_pts = NULL;

    int wall_id = 0; 
    int big_r = 1200;
    int shortest_x = 0;
    int shortest_y = 0;
    float corr = 0.01;

    polyline_t * cur_wall = NULL;
    cur_wall = walls;

    for(; cur_wall; cur_wall=cur_wall->next) {
        // "base" ray - such rays will always at least hit its base end-point.
        float pl_angle = angle(hero_pos.x, hero_pos.y, cur_wall->x1, cur_wall->y1);

        // main ray
        LRE_find_shortest_ray(
            hero_pos.x,                                hero_pos.y,
            cur_wall->x1,                              cur_wall->y1,
            &shortest_x,                               &shortest_y,
            walls,                                     &wall_id
        );
        insert_lightpoint(&light_pts, shortest_x, shortest_y, hero_pos.x, hero_pos.y, wall_id);

        // shifted ray - ray moved by a little angle from "base" ray - this simulates light physics
        // at low cost
        LRE_find_shortest_ray(
            hero_pos.x,                                hero_pos.y,
            hero_pos.x - sin(pl_angle + corr) * big_r, hero_pos.y - cos(pl_angle + corr) * big_r,
            &shortest_x,                               &shortest_y,
            walls,                                     &wall_id
        );
        insert_lightpoint(&light_pts, shortest_x, shortest_y, hero_pos.x, hero_pos.y, wall_id);

        // second shift shifted ray
        LRE_find_shortest_ray(
            hero_pos.x,                                hero_pos.y,
            hero_pos.x - sin(pl_angle - corr) * big_r, hero_pos.y - cos(pl_angle - corr) * big_r,
            &shortest_x,                               &shortest_y,
            walls,                                     &wall_id
        );
        insert_lightpoint(&light_pts, shortest_x, shortest_y, hero_pos.x, hero_pos.y, wall_id);
    }

    LRE_optim_lightpoint(light_pts);
    LRE_draw_dark_sectors(light_pts);

    // LRE_fill_lightpoly(light_pts, hero_pos.x, hero_pos.y);
    // if (DEBUG){LRE_debug_dark_sectors(light_pts);}
    // if (DEBUG){LRE_debug_rays(light_pts, hero_pos.x, hero_pos.y);}

    free_lightpoint(light_pts);
};

// point are sorted by y increasingly, we could use full more sofisticated sorting, but here it is
// so basic that this task handled easily by f-conditions. Points should be sorted in a way that y1
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

polyline_t * init_polyline(int x1, int y1, int x2, int y2) {
    polyline_t * new_polyline = (polyline_t *)malloc(sizeof(polyline_t *));
    new_polyline->x1 = x1;
    new_polyline->y1 = y1;
    new_polyline->x2 = x2;
    new_polyline->y2 = y2;
    new_polyline->next = NULL;

    return new_polyline;
}

void insert_polyline(polyline_t** head, polyline_t* new_polyline) {
    polyline_t** currentRef = head;

    while (*currentRef != NULL) {
        currentRef = &((*currentRef)->next);
    }

    new_polyline->next = *currentRef;
    *currentRef = new_polyline;
}

void free_lightpoint(lightpoint_t* head) 
{
    lightpoint_t* currentRef = head;

    while (currentRef != NULL) {
        lightpoint_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

void free_lightpoly(polyline_t* head) 
{
    polyline_t* currentRef = head;

    while (currentRef != NULL) {
        polyline_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

void free_x_intersection(x_intersection_t* head) 
{
    x_intersection_t* currentRef = head;

    while (currentRef != NULL) {
        x_intersection_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}
