#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "segment.h"

point_t * PT_init(
    int x, int y
)
{
    point_t * new_pt = NULL;

    new_pt = (point_t *)malloc(sizeof(point_t *));

    new_pt->x = x;
    new_pt->y = y;

    return new_pt;
}

segment_t * SEG_init(
    int x1, int y1,
    int x2, int y2
)
{
    segment_t * new_segment = NULL;

    new_segment = (segment_t *)malloc(sizeof(segment_t *));

    new_segment->beg = (point_t){x1, y1};
    new_segment->end = (point_t){x2, y2};

    new_segment->next = NULL;

    return new_segment;
}

void SEG_push(
    segment_t** head,
    int x1, int y1,
    int x2, int y2
)
{
    segment_t * new_segment = NULL;

    new_segment = SEG_init(x1, y1, x2, y2);
    new_segment->next = *head;
    *head = new_segment;
}

void SEG_free(segment_t * segments)
{
    segment_t* head = segments;

    while (head) 
    {
        segment_t * temp = NULL;
        temp = head->next;
        free(head);
        head = temp;
    }
}

int SEG_intersects
(
    segment_t first,
    segment_t second,       
    point_t * intersection    // intersection point is stored here
)
{
    float s_numer, t_numer, denom, t;
    float dx13, dy13;
    float dx21, dx43, dy21, dy43;
    
    dx21 = first.end.x - first.beg.x;
    dy21 = first.end.y - first.beg.y;
    dx43 = second.end.x - second.beg.x;
    dy43 = second.end.y - second.beg.y;

    denom = dx21 * dy43 - dx43 * dy21;

    if (denom == 0)
        return 0; // collinear

    bool denomPositive = denom > 0;

    dx13 = first.beg.x - second.beg.x;
    dy13 = first.beg.y - second.beg.y;

    s_numer = dx21 * dy13 - dy21 * dx13;
    if ((s_numer < 0) == denomPositive)
        return 0; // no collision

    t_numer = dx43 * dy13 - dy43 * dx13;
    if ((t_numer < 0) == denomPositive)
        return 0; // no collision

    if (((s_numer >= denom) == denomPositive) || ((t_numer >= denom) == denomPositive))
        return 0; // no collision

    // collision detected
    t = t_numer / denom;

    intersection->x = (int) first.beg.x + (t * dx21);
    intersection->y = (int) first.beg.y + (t * dy21);

    return 1;
}