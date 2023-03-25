/* list.h -- header file for working with the list*/
#ifndef LIST_H
#define LIST_H
#include <stdbool.h>

struct item
{
    char *path;
};

typedef struct item Item;

typedef struct node
{
    Item item;
    struct node *next;
} Node;

typedef Node *List;

/* This function initializes the list.          */
/* plist is a pointer to list.                  */
void init_list(List *plist);

/* This function returns True if the list       */
/* is empty, and False otherwise.               */
/* plist is a pointer to the initialized list.  */
bool list_is_empty(const List *plist);

/* This function returns True if the list       */
/* is full, and False otherwise.                */
/* plist is a pointer to the initialized list.  */
bool list_is_full(const List *plist);

/* This function returns the number             */
/* of items in the list.                        */
/* plist is a pointer to the initialized list.  */
unsigned int list_item_count(const List *plist);

/* The function frees up the allocated memory   */
void empty_the_list(List *list);

/* The function applies the pfun to each        */
/* element of the plist.                        */
/* plist is a pointer to the initialized list.  */
/* pfun points to a function that takes the     */
/* Item argument and has no return value.       */
void traverse(const List *plist, void (*pfun)(Item item, char *shablon), char *shablon);

/* This function returns True if it was         */
/* possible to add an item to the end           */
/* of list, and False otherwise.                */
/* item - the item to be adde to the list.      */
/* plist is a pointer to the initialized list.  */
bool add_item_end(Item item, List *plist);

/* This function returns True if it was         */
/* possible to add an item to the index n       */
/* of list, and False otherwise.                */
/* plist is a pointer to the initialized list.  */
/* item - the item to be added to the list.     */
/* n is an index starting from 0                */
bool add_n_item(List *plist, Item item, unsigned int n);

/* This function returns True if it was         */
/* possible to add an item to the head          */
/* of list, and False otherwise.                */
/* item - the item to be adde to the list.      */
/* plist is a pointer to the initialized list.  */
bool add_item_head(List *plist, Item item);

void del_last_item(List *plist);

bool del_first_item(List *plist);

bool del_n_item(List *plist, unsigned int n);

#endif /* list.h */