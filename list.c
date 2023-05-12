#include <stdlib.h>
#include <string.h>

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

int cmp(char* s1, char* s2)
{
	return strcmp(s1, s2) < 0;
}

void swap(List** l1, List** l2)
{
	List* temp = *l1;
	*l1 = *l2;
	*l2 = temp;
}

void list_sort(List* list)
{
	char* temp;
	for (List* i = list; i != NULL; i = i->next) {
		List* min = i;
		while (List j = i->next; j != NULL; j = j->next) {
			if (cmp(j->string, min)) {
				min = j;
			}
		}
		swap(i, min);
	}
}
