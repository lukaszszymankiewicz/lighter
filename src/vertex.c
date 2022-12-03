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

    if (candidates) {
        VRTX_free(candidates);
    }
}

bool VRTX_eq(
    vertex_t *first,
    vertex_t *second
) {
    if (VRTX_len(first) != VRTX_len(second)) {
        return false;
    }
    vertex_t *ptr  = NULL;
    vertex_t *ptr2  = NULL;

    while(ptr) {
        if ((ptr->y != ptr2->y) && (ptr->y != ptr2->y)) {
            return false;
        }
        ptr = ptr->next;
        ptr2 = ptr2->next;
    }
    return true;
}

void VRTX_debug(
    vertex_t *vertex
) {
    vertex_t *ptr  = NULL;
    ptr            = vertex;

    while(ptr) {
        printf("x=%d, y=%d \n", ptr->x, ptr->y);
        ptr = ptr->next;
    }
}
