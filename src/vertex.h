#ifndef VERTEX_H
#define VERTEX_H

typedef struct vertex {
    int            x;
    int            y;
    float          angle;
    struct vertex *next;
    float          dist;
} vertex_t;

vertex_t* VRTX_new(int x, int y, float angle);

int VRTX_len(vertex_t *head);

void VRTX_push(vertex_t** head, vertex_t* new_vertex);
void VRTX_delete(vertex_t **head, int y);
void VRTX_add_point(vertex_t **head, int x, int y, float angle);
void VRTX_merge(vertex_t **head, vertex_t  *candidates);
void VRTX_debug(vertex_t *vertex);
void VRTX_delete(vertex_t **head, int y);
void VRTX_free(vertex_t* head);

#endif
