#include <stdlib.h>

#include "global.h"
#include "sorted_list.h"

// intersections are a sorted linked list. It is used for poperly drawing light effect (by
// now). Nothing really special here.
sorted_list_t* SRTLST_new(
    int value,
    int sign
) {
    sorted_list_t* INT_new_sorted_list = (sorted_list_t*)malloc(sizeof(sorted_list_t));
    INT_new_sorted_list->value         = value;
    INT_new_sorted_list->sign          = sign;

    return INT_new_sorted_list;
}

// inserts new x-intersection value and place it in right sorted order
void SRTLST_insert(
    sorted_list_t** head, int value, int sign
) {
    sorted_list_t* current;
    sorted_list_t* new_intersection = SRTLST_new(value, sign);

    if (*head == NULL) {
        new_intersection->next = *head;
        *head = new_intersection;
    }

    // place new point at begininng
    else if ((*head)->value > new_intersection->value) {
        new_intersection->next = *head;
        *head = new_intersection;
    }
    else {
        current = *head;

        while (current->next && current->next->value < new_intersection->value) {
            current = current->next;
        }
        new_intersection->next = current->next;
        current->next = new_intersection;
    }
}

// returns last element from linked list
int SRTLST_get_last(
    sorted_list_t* head
) {
    sorted_list_t* ptr = NULL;
    ptr                = head;

    while(ptr->next) {
        ptr=ptr->next;
    }
    return ptr->value;
}

int SRTLST_len(
    sorted_list_t* head
) {
    sorted_list_t* ptr = NULL;
    ptr                = head;
    int            n   = 0;
    
    while(ptr) {
        n++;
        ptr=ptr->next;
    }

    return n;
}

void SRTLST_free(
    sorted_list_t* head
) {
    if (head == NULL) {
        return;
    }

    sorted_list_t* currentRef = head;

    while (currentRef != NULL) {
        sorted_list_t * temp = currentRef->next;
        free(currentRef);
        currentRef = temp;
    }
}

