#include <stdlib.h>
#include <math.h>
#include "lightpt.h"

// atan2 function from math is used to calculate angle, more naive and faster approach can be taken
// as this is rather slow function. Please note that not direct angle is used in here as it is only
// for sorting purposes - some more naive implementation coudl be easily used.
float LIGPT_calculate_angle(int ax, int ay, int bx, int by) 
{
    return atan2(ax - bx, ay - by);
}

lightpoint_t* LIGPT_new(
    int x,        int y,
    int st_x,     int st_y,
    int wall_id
)
{
    lightpoint_t* new_point = (lightpoint_t*)malloc(sizeof(lightpoint_t));
    new_point->x = x;
    new_point->y = y;
    new_point->wall_id = wall_id;
    new_point->angle = LIGPT_calculate_angle(x, y, st_x, st_y);
    new_point->next = NULL;

    return new_point;
}

// Function to insert a given node at its correct sorted position into a given list sorted in
// increasing order. This is needed to create light of points which creates simple polygon (no
// segments of such polygon won`t intersects with another one if all point are in increasing angle
// order). This works only if we know that we are expecting such polygont from points.
void LIGPT_insert(
    lightpoint_t** head,
    int x1, int y1,
    int x2, int y2,
    int wall_id
) 
{
    lightpoint_t* current;
    lightpoint_t* new_pt = LIGPT_new(x1, y1, x2, y2, wall_id);

    if (*head == NULL)
    {
        new_pt->next = *head;
        *head = new_pt;
    }
    // place new point at begininng
    else if ((*head)->angle >= new_pt->angle)
    {
        new_pt->next = *head;
        *head = new_pt;
    }
    else 
    {
        current = *head;

        while (current->next != NULL && current->next->angle < new_pt->angle) 
        {
            current = current->next;
        }
        new_pt->next = current->next;
        current->next = new_pt;
    }
}

// Some point can be considered redundant. Light polygon is created only from lines that already are
// in the level (walls edges for example). Having three points on one segment makes ht middle point
// really just a waster resources, as intersection needs to be calculated two times.
void LIGPT_optim(lightpoint_t* poly)
{
    lightpoint_t* ptr = NULL;
    ptr = poly;

    // cut all redundant in between
    while (ptr->next != NULL && ptr->next->next != NULL)
    { 
       lightpoint_t* cur_cone = ptr->next;
       if (cur_cone->next->wall_id == ptr->wall_id && cur_cone->next->wall_id == cur_cone->wall_id)
       {
           ptr->next = cur_cone->next; free(cur_cone);
       }
       else 
       { ptr=ptr->next; }
    }
    poly = ptr;
}

void LIGPT_free(lightpoint_t* head) 
{
    lightpoint_t* currentRef = head;

    while (currentRef != NULL) {
        lightpoint_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

