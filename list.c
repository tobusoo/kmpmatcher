#include <stdlib.h>
#include <string.h>

#include "list.h"

List* list_add(List* list, char* str)
{
    List* new_node = malloc(sizeof(List));

    if (new_node) {
        new_node->string = str;
        new_node->next = list;
        list = new_node;
    }

    return list;
}

void list_delete(List* list)
{
    List* next;
    while (list != NULL) {
        next = list->next;
        free(list);
        list = next;
    }
}

static int cmp(char* s1, char* s2)
{
    return strcmp(s1, s2) < 0;
}

static void swap(List* l1, List* l2)
{
    char* temp = l1->string;
    l1->string = l2->string;
    l2->string = temp;
}

void list_sort(List* list)
{
    List* min;
    for (List* i = list; i != NULL; i = i->next) {
        min = i;
        for (List* j = i->next; j != NULL; j = j->next) {
            if (cmp(j->string, min->string)) {
                min = j;
            }
        }
        swap(i, min);
    }
}
