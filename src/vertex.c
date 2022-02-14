#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "global.h"
#include "primitives.h"
#include "vertex.h"

// epsilon value for calculating two segments intersection. The higher the value more edge cases are
// captured
#define EPSILON 0.1

// calculates angle between two points
float VRTX_calculate_angle(int ax, int ay, int bx, int by) {
    return atan2(ax - bx, ay - by);
}

vertex_t* VRTX_new(
    int x,
    int y,
    float angle
) {
    vertex_t* new_vertex = (vertex_t*)malloc(sizeof(vertex_t));
    new_vertex->x        = x;
    new_vertex->y        = y;
    new_vertex->angle    = angle;

    new_vertex->next = NULL;
    new_vertex->prev = NULL;

    return new_vertex;
}

void VRTX_push(
    vertex_t **head,
    vertex_t *new_vertex
) {
    new_vertex->next = *head;
    (*head) = new_vertex;
    (*head)->prev = new_vertex;
}

// Function to insert a given vertex at its correct position into a vertex list with increasing
// order (angle is used as sorting paramter). This is needed for creating points of simple convex
// polygon.
void VRTX_add_point(
    vertex_t **head,
    int x,
    int y,
    float angle
) {
    vertex_t* current    = NULL;
    vertex_t* new_vertex = NULL;
    new_vertex = VRTX_new(x, y, angle);

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

// checks is lightpoint is inside segment, as segments are only vertical and horizontal, simple
// tests is made
bool VRTX_pt_in_segment(
    int pt_x,   int pt_y,
    int seg_x1, int seg_y1,
    int seg_x2, int seg_y2
) {
    if ((pt_x == seg_x1 && pt_x == seg_x2) || (pt_y == seg_y1 && pt_y == seg_y2)) {
        return true;
    }
    else {
        return false;
    }
}

// Some point can be considered redundant. Light polygon is created mostly from lines that already
// are in the level (walls edges for example). Having three points laying in one segment makes the
// middle point really just a waste resources, as intersection needs to be calculated two times.
void VRTX_optim(
    vertex_t *poly
) {
    vertex_t *ptr   = NULL;
    vertex_t *first = NULL;

    first = poly;  // just for the checking optimimalisation between last and the first vertex
    ptr   = poly;

    // cut all redundant in between
    while (ptr->next != NULL && ptr->next->next != NULL) { 
        vertex_t *between = ptr->next;
        vertex_t *prev    = ptr;
        vertex_t *next    = ptr->next->next;

        if(VRTX_pt_in_segment(between->x, between->y, prev->x, prev->y, next->x, next->y)) {
            ptr->next = between->next;
            free(between);
        }
        else {
            ptr=ptr->next; 
        }
    }

    // last point to first point
    vertex_t *between = ptr->next;
    vertex_t *prev    = ptr;
    vertex_t *next    = first;

    if(VRTX_pt_in_segment(between->x, between->y, prev->x, prev->y, next->x, next->y)) {
        ptr->next = NULL;
    }
    
    // new poly is set to optimized set of points
    poly = ptr;
}

// find highest value of y from list of vertices
int VRTX_highest_y(
    vertex_t* poly
) {
    vertex_t *ptr        = NULL;
    int       highest_y  = SCREEN_HEIGHT;

    ptr = poly;

    while(ptr) {
        if (ptr->y < highest_y) {
            highest_y = ptr->y;
        }
        ptr = ptr->next;
    }

    return highest_y;
}

int VRTX_max_y(
    vertex_t* vertex
) {
    if (vertex->next==NULL) {
        return vertex->y;
    }
    else {
        return MAX(vertex->y, vertex->next->y);
    }
}

void VRTX_delete(
    vertex_t **head,
    int          y
) {
    vertex_t *ptr  = NULL;
    vertex_t *prev = NULL;
    ptr            = (*head);

    while(ptr) {
        if (VRTX_max_y(ptr) <= y) {
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
        if (ptr==NULL) {
            return;
        }
        ptr = ptr->next;
    }
}

int VRTX_len(
    vertex_t *head
) {
    int len        = 0;
    vertex_t *ptr  = NULL;

    ptr = head;

    while(ptr) {
        len++;
        ptr=ptr->next;
    }
    return len;
}

void VRTX_free(
    vertex_t* head
) {
    vertex_t* currentRef = NULL;
    currentRef           = head;

    vertex_t* temp = NULL;

    while (currentRef != NULL) {
        vertex_t* temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
    free(temp);
}

