#ifndef SEGMENT_H
#define SEGMENT_H

typedef struct segment {
    int x1;
    int y1;
    int x2;
    int y2;
    struct segment* next;
} segment_t;

void SEG_push(segment_t** head, short int x1, short int y1, short int x2, short int y2);
void SEG_free(segment_t * segments);

#endif
