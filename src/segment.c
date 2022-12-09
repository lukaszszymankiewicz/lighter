#include "geometry.h"
#include "segment.h"

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

void SEG_push(
    segment_t **head,
    int          x1,
    int          y1,
    int          x2,
    int          y2
) {
    segment_t *new_segment  = NULL;
    segment_t *ptr          = NULL;

    new_segment             = (segment_t*)malloc(sizeof(segment_t));
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


int SEG_dist_to_pt(
    segment_t* seg,
    int x,
    int y
) {
    if (seg->type == VER) {
        return abs(x - seg->x1);
    } else {
        return abs(y - seg->y1);
    }
}

// gets segment closest to given point
segment_t* SEG_closest_to_pt(
    segment_t *seg,
    int type,
    int x,
    int y
) {
    int best = 9999;
    int approx_dist;

    segment_t* best_seg = NULL;
    segment_t* ptr = NULL;

    ptr = seg;

    if (!ptr) {
        return NULL;
    }

    for(; ptr; ptr=ptr->next) {
        if (ptr->type == type) {
            approx_dist = SEG_dist_to_pt(ptr, x, y);
        } 

        if (approx_dist < best) {
            best = approx_dist;
            best_seg = ptr;
        }
    }

    return best_seg;
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
