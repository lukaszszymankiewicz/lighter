#ifndef SORTED_LIST_H
#define SORTED_LIST_H

typedef struct sorted_list {
    int                 value;
    int                 sign;
    struct sorted_list *next;
} sorted_list_t;

void SRTLST_insert(sorted_list_t** head, int value, int sign);
void SRTLST_free(sorted_list_t* head);

int SRTLST_get_last(sorted_list_t* head);

#endif
