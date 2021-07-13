#ifndef LIGHTPT_H
#define LIGHTPT_H

typedef struct lightpoint {
    int x;
    int y;
    int wall_id;
    float angle;
    struct lightpoint* next;
} lightpoint_t;

float LIGPT_calculate_angle(int ax, int ay, int bx, int by);
lightpoint_t* LIGPT_new(short int x, short int y, float angle, int wall_id);
void LIGPT_insert(lightpoint_t** head, short int x1, short int y1, float angle, short int wall_id); 
void LIGPT_optim(lightpoint_t* poly);
void LIGPT_free(lightpoint_t* head);

#endif
