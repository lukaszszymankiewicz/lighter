#include <stdio.h>
#include <stdlib.h>

#include "geometry.h"
#include "segment.h"

int SEG_determine_type(
    segment_t* seg
) {
    if (seg->x1 == seg->x2) {
        return VER;
    }

    else if (seg->y1 == seg->y2) {
        return HOR;
    }
    return UNKNOWN;
}

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
    new_segment->type  = SEG_determine_type(new_segment);

    return new_segment;
}
void SEG_push(
    segment_t **head,
    int          x1,
    int          y1,
    int          x2,
    int          y2
) {
    segment_t *new_segment  = NULL;
    segment_t *ptr          = NULL;

    new_segment             = SEG_init(x1, y1, x2, y2);

    if ((*head) == NULL) {
        (*head) = new_segment;
    }
    else if ((*head)->next == NULL) {
        (*head)->next = new_segment;
    }
    else {
        ptr                     = (*head);
        while(ptr->next) {
            ptr=ptr->next;
        }
        ptr->next = new_segment;
    }
}

segment_t* SEG_get(
    segment_t *segments, int index
) {
    segment_t *ptr  = NULL;
    ptr             = segments;
    int         i   = 0;

    while (i < index) {
        ptr=ptr->next;
        i++;
    }
    return ptr;
}

int SEG_len(
    segment_t *segments
) {
    segment_t *ptr = NULL;
    ptr             = segments;
    int         i   = 0;

    while (ptr) {
        ptr=ptr->next;
        i++;
    }

    return i;
}

void SEG_merge(
    segment_t **head,
    segment_t *candidates
) {
    segment_t *ptr = NULL;
    ptr            = candidates;

    while(ptr) {
        SEG_push(head, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
        ptr = ptr->next;
    }
}

void SEG_delete(
    segment_t **head,
    int          y
) {
    segment_t *ptr   = NULL;
    segment_t *prev  = NULL;
    segment_t *temp  = NULL;

    ptr              = (*head);

    while(ptr) {
        if (MAX(ptr->y1, ptr->y2) <= y ) {
            if (!prev) {
                temp = ptr;
                ptr=ptr->next;
                (*head) = ptr;
                free(temp);
                temp = NULL;
            }
            else {
                temp = ptr;

                if (!prev->next) {
                    free(temp);
                    break;
                }
                prev->next = ptr->next;
                ptr = prev->next;
                free(temp);
                temp = NULL;
            }
        } else {
            prev = ptr;
            ptr = ptr->next;
        }
    }
}

int SEG_common_x(
    segment_t *first,
    segment_t *second
) {
    if (first->x1 == second->x1) {
        return first->x1;
    } else if (first->x2 == second->x2) {
        return first->x2;
    } else if (first->x1 == second->x2) {
        return first->x1;
    } else if (first->x2 == second->x1) {
        return first->x2;
    } else {
        return -1;
    }
}

int SEG_common_y(
    segment_t *first,
    segment_t *second
) {
    if (first->y1 == second->y1) {
        return first->y1;
    } else if (first->y2 == second->y2) {
        return first->y2;
    } else if (first->y1 == second->y2) {
        return first->y1;
    } else if (first->y2 == second->y1) {
        return first->y2;
    } else {
        return -1;
    }
}

// checks if all segments in seg contains given point
bool SEG_contains(
    segment_t *seg,
    int x,
    int y
) {
    segment_t* ptr = NULL;
    ptr = seg;

    if (!ptr) {
        return false;
    }

    for(; ptr; ptr=ptr->next) {
        if ((ptr->x1 != x || ptr->y1 != y) && (ptr->x2 != x || ptr->y2 != y)) {
            return false;
        }
    }

    return true;
}

void SEG_free(
    segment_t *head
) {
    segment_t *temp = NULL;

    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

segment_t* SEG_filter_by_rect(
    segment_t* obstacles,
    int r_x1,
    int r_y1,
    int r_x2,
    int r_y2
) {
    segment_t *obs = NULL;
    segment_t *ptr = NULL;

    ptr = obstacles;

    while(ptr) {
        if (GEO_seg_in_rect(
                ptr->x1, ptr->y1,
                ptr->x2, ptr->y2, 
                r_x1, r_y1,
                r_x2, r_y2 != -1)
            )
        SEG_push(
            &obs,
            ptr->x1, ptr->y1,
            ptr->x2, ptr->y2 
        );
            ptr=ptr->next;
    }
    
    return obs;
}

void SEG_debug(
    segment_t* seg
) {
    segment_t* ptr = NULL;
    ptr = seg;

    while(ptr) {
        printf("x1=%d, y1=%d, x2=%d, y2=%d \n", ptr->x1, ptr->y1, ptr->x2, ptr->y2); 
        ptr=ptr->next;
    }
}
