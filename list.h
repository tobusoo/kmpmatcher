#pragma once

typedef struct list {
	char* string;
	struct list* next;
} List;

void list_init(List* list);
void list_add(List* list, char* str);
void list_delete(List* list);
void list_traverse(List* list, void(*pfun)(char* str));
