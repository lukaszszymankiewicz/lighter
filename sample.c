#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "math.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 320                          // screen width in pixels
#define SCREEN_HEIGHT 200                         // screen height in pixels

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct x_intersection {
    int x;
    struct x_intersection* next;
} x_intersection_t;

// single linked list of obstacle segments
typedef struct obstacle {
    int x1; int y1;            // first point
    int x2; int y2;            // second point
    float slope;               // slope coeficient
    struct obstacle* next;     // next obstacle
} obstacle_t;

// ray is represented by segment
typedef struct ray {
    int x1; int y1;
    int x2; int y2;
} ray_t;

typedef struct vertex {
    int x; int y;         // coords of vertex
    float angle;          // angle it makes with player
    float slope;          // slope vertex made with next vertex
    struct vertex* next;  // next vertex
    struct vertex* prev;  // prev vertex
} vertex_t;
#define EPSILON 0.1


float OBS_calc_slope(obstacle_t* obstacle){
    if (obstacle->x1 == obstacle->x2 || obstacle->y1 == obstacle->y2) {
        return 0.0;
    }

    return (float)(obstacle->x2 - obstacle->x1) / (float)(obstacle->y2 - obstacle->y1);
}


obstacle_t* OBS_init(
    int x1, int y1,
    int x2, int y2
)
{
    obstacle_t* new_obstacle = NULL;

    new_obstacle = (obstacle_t *)malloc(sizeof(obstacle_t));

    new_obstacle->x1 = x1;
    new_obstacle->y1 = y1;
    new_obstacle->x2 = x2;
    new_obstacle->y2 = y2;

    new_obstacle->slope = OBS_calc_slope(new_obstacle);

    new_obstacle->next = NULL;

    return new_obstacle;
}

// this push new obstacle to begginig of polygon
void OBS_push(
    obstacle_t** head,
    int x1, int y1,
    int x2, int y2
)
{
    obstacle_t* new_obstacle = NULL;
    new_obstacle = OBS_init(x1, y1, x2, y2);
    
    new_obstacle->next = *head;
    *head = new_obstacle;
}


obstacle_t* OBS_find_candidates(obstacle_t** head, int y){
    // gets all obstacles which y_min is equal to scan_y, deltes it from linked list and return
    obstacle_t* ptr        = NULL;
    obstacle_t* prev       = NULL;
    obstacle_t* candidates = NULL;

    ptr = (*head);

    while(ptr) {
        if (MIN(ptr->y1, ptr->y2) == y) {
            if (prev == NULL) {
                OBS_push(&candidates, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
                ptr=ptr->next;
                (*head) = ptr;
            }
            else {
                OBS_push(&candidates, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
                prev->next = ptr->next;
                free(ptr);
                ptr = prev->next;
            }
        }
        else {
            prev = ptr;
            ptr  = ptr->next;
        }
    }

    return candidates;
}

void OBS_merge(obstacle_t** head, obstacle_t* candidates){
    obstacle_t* ptr = NULL;
    ptr = candidates;

    while(ptr) {
        OBS_push(head, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
        ptr = ptr->next;
    }
}

void OBS_delete(obstacle_t** head, int y){
    obstacle_t* ptr  = NULL;
    obstacle_t* prev = NULL;

    ptr = (*head);

    while(ptr) {
        if (MAX(ptr->y1, ptr->y2) <= y ) {
            if (prev == NULL) {
                ptr=ptr->next;
                (*head) = ptr;
            }
            else {
                prev->next = ptr->next;
                free(ptr);
                ptr = prev->next;
            }

        }

        prev = ptr;
        if (ptr==NULL){
            return;
        }
        ptr = ptr->next;
    }
}

obstacle_t* VRTX_get_obstacles_from_polygon(vertex_t* poly){
    // transform vertices into list of obstacles
    vertex_t* ptr=poly;
    obstacle_t* obstacles = NULL;

    int first_x=ptr->x;
    int first_y=ptr->y;

    while(ptr->next){
        OBS_push(&obstacles, ptr->x, ptr->y, ptr->next->x, ptr->next->y);
        ptr=ptr->next;
    }

    OBS_push(&obstacles, ptr->x, ptr->y, first_x, first_y);

    return obstacles;
}

void OBS_free(obstacle_t* obstacles) {
    obstacle_t* head = obstacles;

    while (head) {
        obstacle_t * temp = NULL;
        temp = head->next;
        free(head);
        head = temp;
    }
}

float GEO_calculate_angle(int ax, int ay, int bx, int by) {
    return atan2(ax - bx, ay - by);
}

// aux for GEO_pt_in_triangle function.
float GEO_sign (
    int x1, int y1,
    int x2, int y2,
    int x3, int y3
)
{
    return (x1 - x3) * (y2- y3) - (x2 - x3) * (y1 - y3);
}

// checks if given point is inside of the triangle
bool GEO_pt_in_triangle (
    int pt_x, int pt_y,    // point we`re looking for
    int t1_x, int t1_y,    // triangle
    int t2_x, int t2_y,    // triangle
    int t3_x, int t3_y     // triangle
)
{
    float d1, d2, d3;
    bool has_neg, has_pos;

    d1 = GEO_sign(pt_x, pt_y, t1_x, t1_y, t2_x, t2_y);
    d2 = GEO_sign(pt_x, pt_y, t2_x, t2_y, t3_x, t3_y);
    d3 = GEO_sign(pt_x, pt_y, t3_x, t3_y, t1_x, t1_y);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

// Checks where y-line intersect with given segment. As y coord of such point is known from the
// begginig - function only return x-coord of such intersection point.
float GEO_seg_intersection_with_y
(
    int y,                   // y-line coord
    int x1,      int y1,     // segment begginig
    int x2,      int y2      // segment end
)
{
    return x1 + (x2 - x1) * (y - y1) / (y2 - y1);
}

// Checks where y-line intersect with given segment. As y coord of such point is known from the
// begginig - function only return x-coord of such intersection point.
float GEO_calc_intersection_with_slope
(
    int y,                   // y-line coord
    int x1,      int y1,     // segment begginig
    float slope
)
{
    return x1 + (y - y1) * slope;
}

// Checks if y-line intersects with given segment. Please note that to do that only y-coord of
// segment is needed to be known. Function will work no matter what is the order of the segment
// points. Note that there is not strict condition from one side of segment end:
// (y1 < scan_y rather than y1 <= scan_y)
bool GEO_if_seg_intersect_with_y
(
    int scan_y,             // y-line coord
    int y1,                 // segment beg y-coords
    int y2                  // segment end y-coord
)
{
    return ((y1 < scan_y && y2 >= scan_y) || (y2 < scan_y && y1 >= scan_y));
}

// Calculates distance between two points. Simple equations is used, can be optimised for something
// better as this is only use to distinguish which point is closer to (0, 0) point.
float GEO_distance(
    int x1, int y1,         // first point
    int x2, int y2          // second point
)
{
    int dx = x1-x2;
    int dy = y1-y2;

    return sqrt(dx*dx + dy*dy);
}

// vertex is created from ray of light, by getting all ends of rays we get polygon which is equal to
// light sight
vertex_t* VRTX_new(
    int start_x, int start_y,    // starting point - for determing angle vertex made
    int x,       int y           // vertex coord
)
{
    vertex_t* new_vertex = (vertex_t*)malloc(sizeof(vertex_t));
    new_vertex->x = x;
    new_vertex->y = y;
    new_vertex->angle = GEO_calculate_angle(start_x, start_y, x, y);

    new_vertex->next = NULL;
    new_vertex->prev = NULL;

    return new_vertex;
}

void VRTX_push(
    vertex_t** head,
    vertex_t* new_vertex
) 
{
    new_vertex->next = *head;
    (*head) = new_vertex;
    (*head)->prev = new_vertex;
}

// Function to insert a given point at its correct sorted position into a given list sorted in
// increasing order (angle is used as sorting paramter). This is needed for creating points of
// simple convex polygon (no segments of such polygon won`t intersects with another).
void VRTX_insert(
    vertex_t** head,
    ray_t* ray
) 
{
    vertex_t* current = NULL;
    vertex_t* new_vertex = NULL;
    new_vertex = VRTX_new(ray->x1, ray->y1, ray->x2, ray->y2);

    if (!(*head)) {
        new_vertex->next = *head;
        *head = new_vertex;
    }

    // place new point at begininng
    else if ((*head)->angle >= new_vertex->angle) {
        new_vertex->next = *head;
        *head = new_vertex;
    }
    else {
        current = *head;

        while (current->next && current->next->angle < new_vertex->angle) {
            current = current->next;
        }
        new_vertex->next = current->next;
        current->next = new_vertex;
    }
}

// checks is lightpoint is inside segment
bool VRTX_pt_in_segment(
    int pt_x, int pt_y,
    int seg_x1, int seg_y1,
    int seg_x2, int seg_y2
)
{
    float cross, dot, box;

    cross = (pt_y - seg_y1) * (seg_x2 - seg_x1) - (pt_x - seg_x1) * (seg_y2 - seg_y1);

    // compare versus epsilon for floating point values, or != 0 if using integers
    if (abs(cross) > EPSILON) {return false;}

    dot = (pt_x - seg_x1) * (seg_x2 - seg_x1) + (pt_y - seg_y1) * (seg_y2 - seg_y1);

    if (dot < 0) {return false;}

    box = (seg_x2 - seg_x1) * (seg_x2 - seg_x1) + (seg_y2 - seg_y1) * (seg_y2 - seg_y1);

    if (dot > box) {return false;}

    return true;
}

float VRTX_calc_slope(vertex_t* first, vertex_t* second){
    if (first->x == second->x) {
        return 0.0;
    }
    return (second->y - first->y) / (second->x - first->x);
}

// calculates slopes between each vertices
void VRTX_complete_vertex(
    vertex_t* vertices
)
{
    vertex_t* ptr = NULL;
    vertex_t* first = NULL;
    vertex_t* prev = NULL;

    first = vertices;
    ptr = vertices;
    while(ptr->next){
        ptr->slope = VRTX_calc_slope(ptr->next, ptr);
        ptr->prev = prev;
        prev=ptr;
        ptr=ptr->next;
    }

    // last vertex
    ptr->slope = VRTX_calc_slope(first, ptr);
    ptr->prev = prev;
}


// Some point can be considered redundant. Light polygon is created mostly from lines that already
// are in the level (walls edges for example). Having three points laying in one segment makes the
// middle point really just a waste resources, as intersection needs to be calculated two times.
void VRTX_optim(vertex_t* poly) {
    vertex_t* ptr = NULL;
    vertex_t* first = NULL;
    first = poly;  // yeah, just for the checking of first to last point
    ptr = poly;

    // cut all redundant in between
    while (ptr->next != NULL && ptr->next->next != NULL) { 
        vertex_t* between = ptr->next;
        vertex_t* prev = ptr;
        vertex_t* next = ptr->next->next;

        if(VRTX_pt_in_segment(between->x, between->y, prev->x, prev->y, next->x, next->y)) {
            ptr->next = between->next;
            free(between);
        }
        else {
            ptr=ptr->next; 
        }
    }

    // last point to first point
    vertex_t* between = ptr->next;
    vertex_t* prev = ptr;
    vertex_t* next = first;

    if(VRTX_pt_in_segment(between->x, between->y, prev->x, prev->y, next->x, next->y)) {
        ptr->next = NULL;
    }
    
    // new poly is set to optimized set of points
    poly = ptr;
}

void VRTX_free(vertex_t* head) {
    vertex_t* currentRef = head;

    while (currentRef != NULL) {
        vertex_t* temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

x_intersection_t* INTSC_new(int x) {
    x_intersection_t* INT_new_x_intersection = (x_intersection_t*)malloc(sizeof(x_intersection_t));
    INT_new_x_intersection->x = x;

    return INT_new_x_intersection;
}

// inserts new x-intersection value and place it in right sorted order
void INTSC_insert(x_intersection_t** head, int x) {
    x_intersection_t* current;
    x_intersection_t* new_intersection = INTSC_new(x);

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

// returns last element from linked list
int INTSC_get_last(x_intersection_t* intersections)
{
    x_intersection_t* ptr = NULL;
    ptr = intersections;

    while(ptr->next) 
    {
        ptr=ptr->next;
    }
    return ptr->x;
}

void INTSC_free(x_intersection_t* head) 
{
    x_intersection_t* currentRef = head;

    while (currentRef != NULL) {
        x_intersection_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

void GFX_draw_colored_line(
    SDL_Renderer *renderer,
    float x1, float y1,
    float x2, float y2,
    int r, int g, int b, int a
){
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
};

void GFX_draw_colored_rect(
    SDL_Renderer *renderer,
    float x1, float y1,
    float w, float h,
    int r, int g, int b, int a
){
    SDL_SetRenderDrawColor(renderer, r, g, b, a);     
    SDL_Rect rect = (SDL_Rect){x1, y1, w, h};
    SDL_RenderFillRect(renderer, &rect);
};

void GFX_draw_scanline(SDL_Renderer *renderer, obstacle_t* lines, int y, int clr){
    int x;
    int n=0;
    int last_intersection;
    int curr_col = 0;
    int cur_alpha = 0;

    // ograj poziome linie
    if (lines){
        obstacle_t* ptr = lines;
        x_intersection_t* points = NULL;

        while(ptr){
            if (ptr->y1 == ptr->y2){
                INTSC_insert(&points, ptr->x1);
                INTSC_insert(&points, ptr->x2);
                ptr=ptr->next;
                n=n+2;
            }
            else {
                x = GEO_calc_intersection_with_slope(y, ptr->x1, ptr->y1, ptr->slope);
                INTSC_insert(&points, x);
                ptr=ptr->next;
                n++;
            }
        }
        
        x_intersection_t* ptr2 = NULL;

        printf("y = %i \n", y);
        ptr2= points;
        while(ptr2){
            printf("%i, ", ptr2->x);
            ptr2=ptr2->next;
        }
        printf("\n\n");


        // dark segment from left side of screen to first intersection with scanline
        if (points->x > 1) {
            GFX_draw_colored_line(renderer, 0, y, points->x, y, 20, 20, 20, 100);
        }

        // dark segment from right side of screen to last intersection with scanline
        last_intersection = INTSC_get_last(points);

        if (last_intersection < SCREEN_WIDTH-1) {
            GFX_draw_colored_line(renderer, last_intersection, y, SCREEN_WIDTH, y, 20, 20, 20, 100);
        }

        if (n==2) {
            GFX_draw_colored_line(renderer, points->x, y, points->next->x, y, 169, 169, 169, clr); 
            return;
        }

        // rest of the lines (light and dark sectors)
        if (n>2) {
            x_intersection_t* ptr = NULL;
            ptr = points;
            curr_col = 169;
            cur_alpha = clr;

            while (ptr){
                GFX_draw_colored_line(
                    renderer, points->x, y, points->next->x, y, curr_col, curr_col, curr_col, cur_alpha
                ); 

                // swith colors
                if (curr_col == 169){
                    curr_col = 20;
                    cur_alpha = 100;
                }
                else {
                    curr_col = 169;
                    cur_alpha = clr;
                }

                if (ptr->next == NULL) {
                    break;
                }

                ptr=ptr->next;

            }
        }

        // last of the dark sector from intesection points
        // GFX_draw_colored_line(points->x, y, points->next->x, y, 169, 169, 169, clr); 
        INTSC_free(points);
    }
    else {
        GFX_draw_colored_line(renderer, 0, y, SCREEN_WIDTH, y, 20, 20, 20, 100);
        return;
    }
}

void GFX_draw_polygon(SDL_Renderer* renderer, vertex_t* poly, int clr){
    int y =0;

    vertex_t* ptr2 = poly;
    printf("vetices: \n");
    while(ptr2){
        printf("(%i, %i) \n\n", ptr2->x, ptr2->y);
        ptr2=ptr2->next;
    }

    obstacle_t* not_drawn_yet = VRTX_get_obstacles_from_polygon(poly); 
    obstacle_t* current_draw = NULL;
    obstacle_t* obstacle_ptr = NULL;
    obstacle_t* candidates = NULL;

    obstacle_t* ptr = not_drawn_yet;
    while(ptr){
        printf("(%i, %i), (%i, %i) \n\n", ptr->x1, ptr->y1, ptr->x2, ptr->y2);
        ptr=ptr->next;
    }

    while(y<SCREEN_HEIGHT) {

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

        GFX_draw_scanline(renderer, current_draw, y, clr);
        y++;
    }

    // fill the rest with darkness (sounds deep)
    GFX_draw_colored_rect(renderer, 0, y, SCREEN_WIDTH, SCREEN_HEIGHT - y, 20, 20, 20, 100);

}

int main(int argc, char* argv[]) {
    // preparations
    vertex_t *poly = NULL;
    ray_t ray;

    ray = (ray_t){157, 143, 106, 118};
    VRTX_insert(&poly, &ray);

    ray = (ray_t){157, 143, 106, 128};
    VRTX_insert(&poly, &ray);

    ray = (ray_t){157, 143, 74, 128};
    VRTX_insert(&poly, &ray);

    ray = (ray_t){157, 143, 42, 123};
    VRTX_insert(&poly, &ray);

    ray = (ray_t){157, 143, 42, 157};
    VRTX_insert(&poly, &ray);

    ray = (ray_t){157, 143, 127, 157};
    VRTX_insert(&poly, &ray);

    ray = (ray_t){157, 143, 157, 143};
    VRTX_insert(&poly, &ray);

    // SDL part
    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        window = SDL_CreateWindow(
            "Lighter",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            1366,
            768,
            SDL_WINDOW_FULLSCREEN
        );

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        SDL_bool done = SDL_FALSE;

        while (!done) {

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

            GFX_draw_polygon(renderer, poly, 10);

            SDL_RenderPresent(renderer);
            SDL_Delay(100);

            SDL_Event event;
            SDL_RenderPresent(renderer);

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = SDL_TRUE;
                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;
}
