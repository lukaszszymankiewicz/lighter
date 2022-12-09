#ifndef SORTED_LIST_H
#define SORTED_LIST_H

typedef struct sorted_list {
    int                 value;
    struct sorted_list *next;
} sorted_list_t;

void SRTLST_insert(sorted_list_t** head, int value);
void SRTLST_free(sorted_list_t* head);

int SRTLST_get_last(sorted_list_t* head);
int SRTLST_len (sorted_list_t* head);

#endif
