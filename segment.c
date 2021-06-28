#include <stdlib.h>
#include "segment.h"

segment_t * SEG_init(
    short int x1, short int y1,
    short int x2, short int y2
)
{
    segment_t * new_segment = NULL;
    new_segment = (segment_t *)malloc(sizeof(segment_t *));

    new_segment->x1 = x1;
    new_segment->y1 = y1;
    new_segment->x2 = x2;
    new_segment->y2 = y2;

    new_segment->next = NULL;

    return new_segment;
}

void SEG_push(
    segment_t** head,
    short int x1, short int y1,
    short int x2, short int y2
)
{
    segment_t * new_segment = NULL;

    new_segment = SEG_init(x1, y1, x2, y2);
    new_segment->next = *head;
    *head = new_segment;
}

void SEG_free(segment_t * segments)
{
    segment_t* currentRef = segments;

    while (currentRef != NULL) 
    {
        segment_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }

}

