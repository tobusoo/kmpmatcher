#include <stdlib.h>

#include "list.h"

void list_init(List* list, char* str)
{
	list->next = NULL;
	list->string = str;
}

void list_add(List* list, char* str)
{
	List* new_node = malloc(sizeof(List));
	if (new_node != NULL) {
		new_node->string = str;
		new_node->next = list;
		list = new_node;			
	}
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

void list_traverse(List* list, void(*pfun)(char* str))
{
	List* node = list;
	while (node != NULL) {
		(*pfun)(str);
		node = node->str;
	}
}
