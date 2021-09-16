#include "intersection.h"

// intersections are a simple sorted linked list. It is used for poperly drawing light effect (by
// now). Nothing really special here.

x_intersection_t* INTSC_new(int x) {
    x_intersection_t* INT_new_x_intersection = (x_intersection_t*)malloc(sizeof(x_intersection_t));
    INT_new_x_intersection->x = x;

    return INT_new_x_intersection;
}

// inserts new x-intersection value and place it in right sorted order
void INTSC_insert(x_intersection_t** head, int x) {
    x_intersection_t* current;
    x_intersection_t* new_intersection = INTSC_new(x);

    if (*head == NULL)
    {
        new_intersection->next = *head;
        *head = new_intersection;
    }
    // place new point at begininng
    else if ((*head)->x >= new_intersection->x)
    {
        new_intersection->next = *head;
        *head = new_intersection;
    }
    else 
    {
        current = *head;

        while (current->next != NULL && current->next->x < new_intersection->x) 
        {
            current = current->next;
        }
        new_intersection->next = current->next;
        current->next = new_intersection;
    }
}

// returns last element from linked list
int INTSC_get_last(x_intersection_t* intersections)
{
    x_intersection_t* ptr = NULL;
    ptr = intersections;

    while(ptr->next) 
    {
        ptr=ptr->next;
    }
    return ptr->x;
}

void INTSC_free(x_intersection_t* head) 
{
    x_intersection_t* currentRef = head;

    while (currentRef != NULL) {
        x_intersection_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

