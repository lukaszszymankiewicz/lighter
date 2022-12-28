#include "global.h"
#include "geometry.h"
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

int VRTX_highest_y(
    vertex_t* poly
) {
    vertex_t *ptr        = NULL;
    int       highest_y  = 9999;

    ptr = poly;

    while(ptr) {
        if (ptr->y < highest_y) {
            highest_y = ptr->y;
        }
        ptr = ptr->next;
    }
    return highest_y;
}

int VRTX_lowest_y(
    vertex_t* poly
) {
    vertex_t *ptr        = NULL;
    int       lowest_y  = -9999;

    ptr = poly;

    while(ptr) {
        if (ptr->y > lowest_y) {
            lowest_y = ptr->y;
        }
        ptr = ptr->next;
    }
    return lowest_y;
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

    while (head != NULL) {
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

    if (candidates) {
        VRTX_free(candidates);
    }
}

bool VRTX_contains(
    vertex_t *head,
    int x,
    int y
) {
    vertex_t* ptr = NULL;
    ptr           = head;

    while(ptr) {

        if(ptr->x == x && ptr->y == y) {
            return true; 
        }

        ptr=ptr->next;
    }

    return false;
}

void VRTX_merge_unique(
    vertex_t **head,
    vertex_t  *candidates
) {
    vertex_t *ptr  = NULL;
    ptr            = candidates;

    while(ptr) {
        if (VRTX_contains((*head), ptr->x, ptr->y)) {
            ptr=ptr->next;
            continue;
        }
        
        VRTX_add_point(head, ptr->x, ptr->y, ptr->angle);
        ptr=ptr->next;
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

vertex_t* VRTX_transpose(
    vertex_t *vertex,
    int       x_corr,
    int       y_corr,
    int       x0,
    int       y0
) {
    vertex_t *ptr  = NULL;
    ptr            = vertex;

    while(ptr) {
        ptr->angle = GEO_angle_2pt(x0, y0, ptr->x + x_corr, ptr->y + y_corr);
        ptr->x = ptr->x + x_corr;
        ptr->y = ptr->y + y_corr;
        ptr = ptr->next;
    }

    return vertex;
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

int VRTX_pop_y(
    vertex_t* vertex
) {
    vertex_t *ptr = NULL;
    ptr           = vertex;

    while(ptr->next) {
        ptr = ptr->next;
    }

    return ptr->y;
}

int VRTX_pop_x(
    vertex_t* vertex
) {
    vertex_t *ptr = NULL;
    ptr           = vertex;

    while(ptr->next) {
        ptr = ptr->next;
    }

    return ptr->x;
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
