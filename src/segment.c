#include "def.h"
#include "segment.h"
#include "macros.h"
#include "geometry.h"

segment_t* SEG_init(
    int x1, int y1,
    int x2, int y2
) {
    segment_t* new_segment   = NULL;
    new_segment              = (segment_t *)malloc(sizeof(segment_t));

    new_segment->x1 = x1;
    new_segment->y1 = y1;
    new_segment->x2 = x2;
    new_segment->y2 = y2;

    new_segment->slope = GEO_calc_slope(x1, y1, x2, y2);
    new_segment->next  = NULL;

    return new_segment;
}

// this push new obstacle to begginig of polygon
void SEG_push(
    segment_t **head,
    int          x1,
    int          y1,
    int          x2,
    int          y2
) {
    segment_t *new_segment = NULL;
    new_segment            = SEG_init(x1, y1, x2, y2);
    new_segment->next      = *head;
    *head                  = new_segment;
}


segment_t* SEG_find_candidates(
    segment_t **head,
    int          y
) {
    // gets all obstacles which y_min is equal to scan_y, deletes it from linked list and return
    segment_t* ptr        = NULL;
    segment_t* prev       = NULL;
    segment_t* candidates = NULL;

    ptr = (*head);

    while(ptr) {
        if (MIN(ptr->y1, ptr->y2) == y) {
            if (prev == NULL) {
                SEG_push(&candidates, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
                ptr=ptr->next;
                (*head) = ptr;
            }
            else {
                SEG_push(&candidates, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
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

void SEG_merge(
    segment_t **head,
    segment_t  *candidates
) {
    segment_t *ptr = NULL;
    ptr             = candidates;

    while(ptr) {
        SEG_push(head, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
        ptr = ptr->next;
    }
}

void SEG_delete(
    segment_t **head,
    int          y
) {
    segment_t *ptr  = NULL;
    segment_t *prev = NULL;
    ptr              = (*head);

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

segment_t* SEG_get_segments_of_polygon(
    vertex_t *poly
) {
    // transform vertices into list of obstacles
    vertex_t *ptr         = poly;
    segment_t *segments   = NULL;
    int first_x           = ptr->x;
    int first_y           = ptr->y;

    while(ptr->next){
        SEG_push(&segments, ptr->x, ptr->y, ptr->next->x, ptr->next->y);
        ptr=ptr->next;
    }

    SEG_push(&segments, ptr->x, ptr->y, first_x, first_y);

    return segments;
}

void SEG_free(
    segment_t *segments
) {
    segment_t *head = segments;

    while (head) {
        segment_t * temp = NULL;
        temp = head->next;
        free(head);
        head = temp;
    }
}

