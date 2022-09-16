#include "global.h"
#include "point.h"
#include "primitives.h"

point_t* PT_new(
    int x, int y
) {
    point_t *new_point = NULL;

    new_point          = (point_t*)malloc(sizeof(point_t));
    new_point->x       = x;
    new_point->y       = y;
    new_point->next    = NULL;

    return new_point;
}

bool PT_eq(
    point_t *first,
    point_t *second
) {
    return (first->x == second->x && first->y == second->y);
}

bool PT_contains(
    point_t *head,
    int x,
    int y
) {
    point_t *ptr = NULL;
    ptr = head;

    while (ptr) {
        if (ptr->x == x && ptr->y == y) {
            return true;
        }
        ptr=ptr->next;
    }
    return false;
}

void PT_push(
    point_t **head,
    int          x,
    int          y
) {
    point_t *new_point = NULL;
    point_t *ptr       = NULL;

    new_point          = PT_new(x, y);
    ptr                = (*head);

    // head is empty
    if ((*head) == NULL) {
        (*head) = new_point;
    }
    // next of head is empty
    else if (PT_eq((*head), new_point)) {
        free(new_point);
        return;
    }
    else if ((*head)->next == NULL) {
        (*head)->next = new_point;
    }
    else {
        while(ptr->next) {
            if (PT_eq(ptr->next, new_point)) {
                free(new_point);
                return;
            }
            else {
                ptr=ptr->next;
            }
        }
        ptr->next = new_point;
    }
}

int PT_len(
    point_t *points
) {
    point_t *ptr = NULL;
    ptr          = points;
    int      i   = 0;

    while (ptr) {
        ptr=ptr->next;
        i++;
    }

    return i;
}

void PT_free(
    point_t* head
) {
    point_t* ptr = head;

    while (ptr) {
        point_t *temp = ptr->next;
        free(ptr);
        ptr = temp;
    }
}

void PT_merge(
    point_t **head,
    point_t  *candidates
) {
    point_t *ptr   = NULL;
    ptr            = candidates;

    while(ptr) {
        PT_push(head, ptr->x, ptr->y);
        ptr = ptr->next;
    }

    if (candidates) {
        PT_free(candidates);
    }
}

