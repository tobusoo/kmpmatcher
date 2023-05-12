#pragma once

typedef struct list {
    char* string;
    struct list* next;
} List;

List* list_add(List* list, char* str);
void list_delete(List* list);
void list_sort(List* list);