#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "global.h"
#include "vertex.h"

vertex_t* VRTX_new(
    int x,
    int y,
    float angle
) {
    vertex_t* new_vertex = (vertex_t*)malloc(sizeof(vertex_t));

    new_vertex->x        = x;
    new_vertex->y        = y;
    new_vertex->angle    = angle;
    new_vertex->next     = NULL;

    return new_vertex;
}

// Function to insert a given vertex at its correct position into a vertex list with increasing
// order (angle is used as sorting paramter). This is needed for creating points of simple convex
// polygon.
void VRTX_add_point(
    vertex_t **head,
    int        x,
    int        y,
    float      angle
) {
    vertex_t* current    = NULL;
    vertex_t* new_vertex = NULL;
    new_vertex           = VRTX_new(x, y, angle);

    if ((*head) == NULL) {
        new_vertex->next = (*head);
        (*head)          = new_vertex;
    }

    // place new point at begininng
    else if ((*head)->angle >= new_vertex->angle) {
        new_vertex->next = (*head);
        (*head)          = new_vertex;
    }
    else {
        current = (*head);

        while (current->next && current->next->angle < new_vertex->angle) {
            current = current->next;
        }
        new_vertex->next = current->next;
        current->next    = new_vertex;
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
    vertex_t* temp = NULL;

    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void VRTX_merge(
    vertex_t **head,
    vertex_t  *candidates
) {
    vertex_t *ptr  = NULL;
    ptr            = candidates;

    while(ptr) {
        VRTX_add_point(head, ptr->x, ptr->y, ptr->angle);
        ptr=ptr->next;
    }
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

void VRTX_debug(
    vertex_t *vertex
) {
    vertex_t *ptr = NULL;
    ptr           = vertex;
    int i=0;

    while(ptr) {
        printf("%d) x=%d y=%d \n", i, ptr->x, ptr->y);
        ptr=ptr->next;
        i++;
    }
}
