#include <stdlib.h>
#include <stdio.h>

#include "vertex.h"

#define MAX_ANGLE        10.0
#define COORD_PER_VERTEX 2

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

void VRTX_force_first(
    vertex_t **head,
    int        x,
    int        y
) {
    vertex_t* new_vertex = NULL;
    // gracefully force the vertex to be always first with MAX_ANGLE
    new_vertex           = VRTX_new(x, y, MAX_ANGLE);

    new_vertex->next     = (*head);
    (*head)              = new_vertex;
}

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
    else if ((*head)->angle <= new_vertex->angle) {
        new_vertex->next = (*head);
        (*head)          = new_vertex;
    }
    else {
        current = (*head);

        while (current->next && current->next->angle > new_vertex->angle) {
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

float* VRTX_to_coords(
    vertex_t* vertices
) {
    vertex_t *ptr    = NULL;
    float    *coords = NULL;
    int       j      = 0;

    coords = (float*)malloc(sizeof(float) * VRTX_len(vertices) * COORD_PER_VERTEX);

    for (ptr=vertices; ptr; ptr=ptr->next) {
        coords[j++] = (float)ptr->x;
        coords[j++] = (float)ptr->y;
    }

    // vertices is consumed
    VRTX_free(vertices);

    return coords;
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
