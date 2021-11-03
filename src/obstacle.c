#include "def.h"
#include "config.h"
#include "obstacle.h"
#include "macros.h"
#include "tile.h"


obstacle_t* OBS_init(
    int x1, int y1,
    int x2, int y2
) {
    obstacle_t* new_obstacle   = NULL;
    new_obstacle               = (obstacle_t *)malloc(sizeof(obstacle_t));

    // new_obstacle->x1 = TRIM(0, SCREEN_WIDTH,  x1);
    // new_obstacle->y1 = TRIM(0, SCREEN_HEIGHT, y1);
    // new_obstacle->x2 = TRIM(0, SCREEN_WIDTH,  x2);
    // new_obstacle->y2 = TRIM(0, SCREEN_HEIGHT, y2);

    new_obstacle->x1 =  x1;
    new_obstacle->y1 =  y1;
    new_obstacle->x2 =  x2;
    new_obstacle->y2 =  y2;

    new_obstacle->next  = NULL;

    return new_obstacle;
}

void OBS_push(
    obstacle_t **head,
    int          x1,
    int          y1,
    int          x2,
    int          y2
) {
    obstacle_t *new_obstacle = NULL;
    obstacle_t *ptr          = NULL;

    new_obstacle             = OBS_init(x1, y1, x2, y2);
    ptr                      = (*head);

    if ((*head) == NULL) {
        (*head) = new_obstacle;
    }
    else if ((*head)->next == NULL) {
        (*head)->next = new_obstacle;
    }
    else {
        while(ptr->next) {
            ptr=ptr->next;
        }
        ptr->next = new_obstacle;
    }
}

obstacle_t* OBS_get(
    obstacle_t *obstacles, int index
) {
    obstacle_t *ptr = NULL;
    ptr             = obstacles;
    int         i   = 0;

    while (i < index) {
        ptr=ptr->next;
        i++;
    }

    return ptr;
}

int OBS_len(
    obstacle_t *obstacles
) {
    obstacle_t *ptr = NULL;
    ptr             = obstacles;
    int         i   = 0;

    while (ptr) {
        ptr=ptr->next;
        i++;
    }

    return i;
}

void OBS_free(
    obstacle_t *obstacles
) {
    obstacle_t *head = obstacles;

    while (head) {
        obstacle_t * temp = NULL;
        temp = head->next;
        free(head);
        head = temp;
    }
}
