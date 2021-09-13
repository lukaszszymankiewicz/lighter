#include "math.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

typedef struct light_obstacle {
    int x1; int y1;
    int x2; int y2;
    float slope;
    struct light_obstacle* next;
} light_obstacle_t;


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
// single linked list of obstacle segments
typedef struct obstacle {
    int x1; int y1;            // first point
    int x2; int y2;            // second point
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
} vertex_t;

#define EPSILON 0.1

float VRTX_calc_slope(light_obstacle_t* obstacle){
    if (obstacle->x1 == obstacle->x2) {
        return 0.0;
    }
    return (obstacle->y2 - obstacle->y1) / (obstacle->x2 - obstacle->x1);
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
    new_vertex->slope = -1;      // -1 means slope is not calculated

    new_vertex->next = NULL;

    return new_vertex;
}


void VRTX_append_obstacle(
    light_obstacle_t** head,
    int x1, int y1,
    int x2, int y2
) 
{
    light_obstacle_t* new_obstacle = (light_obstacle_t*)malloc(sizeof(light_obstacle_t));

    new_obstacle->x1 = x1;
    new_obstacle->y1 = y1;
    new_obstacle->x2 = x2;
    new_obstacle->y2 = y2;
    new_obstacle->slope = VRTX_calc_slope(new_obstacle);

    new_obstacle->next = *head;
    (*head) = new_obstacle;
}

void VRTX_add(
    light_obstacle_t** head,
    light_obstacle_t* new_obstacle
) 
{
    new_obstacle->next = *head;
    (*head) = new_obstacle;
}

void VRTX_push_obstacle(
    light_obstacle_t** head,
    light_obstacle_t* ptr
) 
{
    light_obstacle_t* new_obstacle = (light_obstacle_t*)malloc(sizeof(light_obstacle_t));

    new_obstacle->x1 = ptr->x1;
    new_obstacle->y1 = ptr->y1; 

    new_obstacle->x2 = ptr->x2;
    new_obstacle->y2 = ptr->y2; 
    new_obstacle->slope = VRTX_calc_slope(new_obstacle);

    new_obstacle->next = *head;
    (*head) = new_obstacle;
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

void VRTX_print(light_obstacle_t* head) {
    if (!head){
        return;
    }

    light_obstacle_t* temp = NULL;
    temp = head;

    while(temp){
        printf(" (%i, %i)-(%i, %i) ", temp->x1, temp->y1, temp->x2, temp->y2);
        temp=temp->next;
    }
    
}

void VRTX_delete_vertes(vertex_t** head, int y){
    vertex_t* ptr  = NULL;
    vertex_t* prev = NULL;

    ptr = (*head);

    while(ptr && ptr->next) {
        if (MAX(ptr->y, ptr->next->y) == y ) {
            if (prev == NULL) {
                ptr=ptr->next;
                (*head)= ptr;
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
        ptr  = ptr->next;
    }
}

light_obstacle_t* VRTX_find_candidates(light_obstacle_t** head, int y){
    // gets all obstacles which y_min is equal to scan_y, deltes it from linked list and return
    light_obstacle_t* ptr        = NULL;
    light_obstacle_t* prev       = NULL;
    light_obstacle_t* candidates = NULL;

    ptr = (*head);

    while(ptr) {
        if (MIN(ptr->y1, ptr->y2) == y) {
            if (prev == NULL) {
                VRTX_push_obstacle(&candidates, ptr);
                ptr=ptr->next;
                (*head) = ptr;
            }
            else {
                VRTX_push_obstacle(&candidates, ptr);
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

void VRTX_merge(light_obstacle_t** head, light_obstacle_t* candidates){
    light_obstacle_t* ptr = NULL;
    ptr = candidates;

    while(ptr) {
        VRTX_push_obstacle(head, ptr);
        ptr = ptr->next;
    }
}

void VRTX_delete_obstacle(light_obstacle_t** head, int y){
    light_obstacle_t* ptr  = NULL;
    light_obstacle_t* prev = NULL;

    ptr = (*head);

    while(ptr) {
        if (MAX(ptr->y1, ptr->y2) <= y ) {
            if (prev == NULL) {
                // printf("deleted segment: (%i, %i), (%i, %i)\n", ptr->x1, ptr->y1, ptr->x2, ptr->y2);
                ptr=ptr->next;
                (*head) = ptr;
            }
            else {
                // printf("deleted segment: (%i, %i), (%i, %i)\n", ptr->x1, ptr->y1, ptr->x2, ptr->y2);
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

int main() {
    vertex_t *poly = NULL;
    ray_t ray;

    // left
    ray = (ray_t){100, 100, 50, 100};
    VRTX_insert(&poly, &ray);

    ray = (ray_t){100, 100, 75, 90};
    VRTX_insert(&poly, &ray);

    // right
    ray = (ray_t){100, 100, 150, 110};
    VRTX_insert(&poly, &ray);

    // down
    ray = (ray_t){100, 100, 100, 120};
    VRTX_insert(&poly, &ray);

    // up
    ray = (ray_t){100, 100, 100, 80};
    VRTX_insert(&poly, &ray);


    //
    // This is a starting point - we got our polygon ready
    //
     
    // transform vertices into list of obstacles
    vertex_t* ptr2=poly;
    light_obstacle_t* not_drawn_yet = NULL;
    light_obstacle_t* ptr = NULL;

    int first_x=ptr2->x;
    int first_y=ptr2->y;

    while(ptr2->next){
        VRTX_append_obstacle(&not_drawn_yet, ptr2->x, ptr2->y, ptr2->next->x, ptr2->next->y);
        ptr2=ptr2->next;
    }
    // and the last one
    VRTX_append_obstacle(&not_drawn_yet, ptr2->x, ptr2->y, first_x, first_y);

    light_obstacle_t *ptr3=not_drawn_yet;
    while(ptr3){
        // printf("(%i, %i), (%i, %i)\n", ptr3->x1, ptr3->y1, ptr3->x2, ptr3->y2); 
        ptr3=ptr3->next;
    }

    light_obstacle_t* current_draw = NULL;
    vertex_t* waiting_to_be_drawn = NULL;
    light_obstacle_t* obstacle_ptr = NULL;
    light_obstacle_t* candidates = NULL;
    vertex_t* vertex_ptr = NULL;

    for (int y=0; y<130; y++){
        printf("%i = ", y);

        // delete light obstacles current drawn
        // scan_y must be higher than y of any point
        obstacle_ptr = current_draw;
        while(obstacle_ptr){
            VRTX_delete_obstacle(&current_draw, y);
            obstacle_ptr=obstacle_ptr->next;
        }

        // get candidates to draw
        candidates = VRTX_find_candidates(&not_drawn_yet, y);

        // add candidates to current draw
        VRTX_merge(&current_draw, candidates);       

        if (current_draw != NULL) {
            VRTX_print(current_draw);
        }

        if (!not_drawn_yet && !current_draw){
            break;
        }

        printf("\n");
    }
    return 1;
}
