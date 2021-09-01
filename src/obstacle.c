#include "def.h"
#include "obstacle.h"


obstacle_t * OBS_init(
    int x1, int y1,
    int x2, int y2
)
{
    obstacle_t * new_obstacle = NULL;

    new_obstacle = (obstacle_t *)malloc(sizeof(obstacle_t *));

    new_obstacle->x1 = x1;
    new_obstacle->y1 = y2;
    new_obstacle->x2 = x2;
    new_obstacle->y2 = y2;

    new_obstacle->next = NULL;

    return new_obstacle;
}

// this push new obstacle to begginig of polygon
void OBS_push(
    obstacle_t** head,
    int x1, int y1,
    int x2, int y2
)
{
    obstacle_t * new_obstacle = NULL;

    new_obstacle = OBS_init(x1, y1, x2, y2);
    new_obstacle->next = *head;
    *head = new_obstacle;
}


void OBS_free(obstacle_t* obstacles) {
    obstacle_t* head = obstacles;

    while (head) {
        obstacle_t * temp = NULL;
        temp = head->next;
        free(head);
        head = temp;
    }
}

