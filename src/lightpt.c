#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lightpt.h"

#define EPSILON 0.1

// atan2 function from math is used to calculate angle, more naive and faster approach can be taken
// as this is rather slow function. Please note that not direct angle is used in here as it is only
// for sorting purposes - some more naive implementation coudl be easily used.
float LIGPT_calculate_angle(int ax, int ay, int bx, int by) 
{
    return atan2(ax - bx, ay - by);
}

lightpoint_t* LIGPT_new(
    short int x, 
    short int y,
    float angle 
)
{
    lightpoint_t* new_point = (lightpoint_t*)malloc(sizeof(lightpoint_t));
    new_point->x = x;
    new_point->y = y;
    new_point->angle = angle;
    new_point->next = NULL;

    return new_point;
}

// Function to insert a given point at its correct sorted position into a given list sorted in
// increasing order (angle is used as sorting paramter). This is needed for creating points of
// simple convex polygon (no segments of such polygon won`t intersects with another).
void LIGPT_insert(
    lightpoint_t** head,
    short int x,
    short int y,
    float angle 
) 
{
    lightpoint_t* current;
    lightpoint_t* new_pt = LIGPT_new(x, y, angle);

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


bool LIGPT_pt_in_segment(
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
void LIGPT_optim(lightpoint_t* poly)
{
    lightpoint_t* ptr = NULL;
    lightpoint_t* first = NULL;
    first = poly;  // yeah, just for the checking of first to last point
    ptr = poly;

    // cut all redundant in between
    while (ptr->next != NULL && ptr->next->next != NULL)
    { 
        lightpoint_t* between = ptr->next;
        lightpoint_t* prev = ptr;
        lightpoint_t* next = ptr->next->next;

        if(LIGPT_pt_in_segment(between->x, between->y, prev->x, prev->y, next->x, next->y))
        {
            ptr->next = between->next;
            free(between);
        }
        else 
        {
            ptr=ptr->next; 
        }
    }

    // last point to first point
    lightpoint_t* between = ptr->next;
    lightpoint_t* prev = ptr;
    lightpoint_t* next = first;

    if(LIGPT_pt_in_segment(between->x, between->y, prev->x, prev->y, next->x, next->y))
    {
        ptr->next = NULL;
    }
    
    // new poly is set to optimized set of points
    poly = ptr;
}

void LIGPT_free(lightpoint_t* head) 
{
    lightpoint_t* currentRef = head;

    while (currentRef != NULL) {
        lightpoint_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

