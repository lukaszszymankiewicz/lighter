#include "def.h"
#include "obstacle.h"

float OBS_calc_slope(obstacle_t* obstacle){
    if (obstacle->x1 == obstacle->x2) {
        return 0.0;
    }

    return (float)(obstacle->x2 - obstacle->x1) / (float)(obstacle->y2 - obstacle->y1);
}


obstacle_t* OBS_init(
    int x1, int y1,
    int x2, int y2
)
{
    obstacle_t* new_obstacle = NULL;

    new_obstacle = (obstacle_t *)malloc(sizeof(obstacle_t));

    new_obstacle->x1 = x1;
    new_obstacle->y1 = y1;
    new_obstacle->x2 = x2;
    new_obstacle->y2 = y2;

    new_obstacle->slope = OBS_calc_slope(new_obstacle);

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
    obstacle_t* new_obstacle = NULL;
    new_obstacle = OBS_init(x1, y1, x2, y2);
    
    new_obstacle->next = *head;
    *head = new_obstacle;
}


obstacle_t* OBS_find_candidates(obstacle_t** head, int y){
    // gets all obstacles which y_min is equal to scan_y, deltes it from linked list and return
    obstacle_t* ptr        = NULL;
    obstacle_t* prev       = NULL;
    obstacle_t* candidates = NULL;

    ptr = (*head);

    while(ptr) {
        if (MIN(ptr->y1, ptr->y2) == y) {
            if (prev == NULL) {
                OBS_push(&candidates, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
                ptr=ptr->next;
                (*head) = ptr;
            }
            else {
                OBS_push(&candidates, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
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

void OBS_merge(obstacle_t** head, obstacle_t* candidates){
    obstacle_t* ptr = NULL;
    ptr = candidates;

    while(ptr) {
        OBS_push(head, ptr->x1, ptr->y1, ptr->x2, ptr->y2);
        ptr = ptr->next;
    }
}

void OBS_delete(obstacle_t** head, int y){
    obstacle_t* ptr  = NULL;
    obstacle_t* prev = NULL;

    ptr = (*head);

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

obstacle_t* VRTX_get_obstacles_from_polygon(vertex_t* poly){
    // transform vertices into list of obstacles
    vertex_t* ptr=poly;
    obstacle_t* obstacles = NULL;

    int first_x=ptr->x;
    int first_y=ptr->y;

    while(ptr->next){
        OBS_push(&obstacles, ptr->x, ptr->y, ptr->next->x, ptr->next->y);
        ptr=ptr->next;
    }

    OBS_push(&obstacles, ptr->x, ptr->y, first_x, first_y);

    return obstacles;
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

