#include "def.h"
#include "primitives.h"
#include "vertex.h"

#define EPSILON 0.1

// vertex is created from ray of light, by getting all ends of rays we get polygon which is equal to
// light sight

// atan2 function from math is used to calculate angle, more naive and faster approach can be taken
// as this is rather slow function. Please note that not direct angle is used in here as it is only
// for sorting purposes - some more naive implementation coudl be easily used.
float VRTX_calculate_angle(int ax, int ay, int bx, int by) {
    return atan2(ax - bx, ay - by);
}

vertex_t* VRTX_new(
    int x,       int y,         // vertex coord
    int start_x, int start_y    // starting point - for determing angle vertex made
)
{
    vertex_t* new_point = (vertex_t*)malloc(sizeof(vertex_t));
    new_point->x = x;
    new_point->y = y;
    new_point->angle = VRTX_calculate_angle(x, y, start_x, start_y);
    new_point->next = NULL;

    return new_point;
}

// Function to insert a given point at its correct sorted position into a given list sorted in
// increasing order (angle is used as sorting paramter). This is needed for creating points of
// simple convex polygon (no segments of such polygon won`t intersects with another).
void VRTX_insert(
    vertex_t** head,
    ray_t* ray
) 
{
    vertex_t* current;
    vertex_t* new_pt = VRTX_new(ray->x2, ray->y2, ray->x1, ray->y2);

    if (*head == NULL) {
        new_pt->next = *head;
        *head = new_pt;
    }

    // place new point at begininng
    else if ((*head)->angle >= new_pt->angle) {
        new_pt->next = *head;
        *head = new_pt;
    }
    else {
        current = *head;

        while (current->next != NULL && current->next->angle < new_pt->angle) {
            current = current->next;
        }
        new_pt->next = current->next;
        current->next = new_pt;
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
        vertex_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

