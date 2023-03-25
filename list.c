/* list.c -- functions to support working with a list*/
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void copy_to_node(Item item, Node *pnode)
{
    pnode->item = item;
}

void init_list(List *plist)
{
    *plist = NULL;
}

bool list_is_empty(const List *plist)
{
    if (*plist == NULL)
    {
        return true;
    }
    return false;
}

bool list_is_full(const List *plist)
{
    Node *pnode;
    pnode = (Node *)malloc(sizeof(Node));
    if (pnode == NULL)
    {
        return true;
    }
    else
    {
        free(pnode);
        return false;
    }
}

unsigned int list_item_count(const List *plist)
{
    int count = 0;
    Node *pnode = *plist;

    while (pnode != NULL)
    {
        count++;
        pnode = pnode->next;
    }

    return count;
}

bool add_item_end(Item item, List *plist)
{
    Node *pnew;
    Node *scan = *plist;

    pnew = (Node *)malloc(sizeof(Node));
    if (pnew == NULL)
    {
        return false;
    }
    copy_to_node(item, pnew);
    if (scan == NULL)
    {
        *plist = pnew;
    }
    else
    {
        while (scan->next != NULL)
        {
            scan = scan->next;
        }
        scan->next = pnew;
    }

    return true;
}

void empty_the_list(List *plist)
{
    Node *next;
    while (*plist != NULL)
    {
        next = (*plist)->next;
        free(*plist);
        *plist = next;
    }
}

void traverse(const List *plist, void (*pfun)(Item item, char *shablon), char *shablon)
{
    Node *pnode = *plist;
    while (pnode != NULL)
    {
        (*pfun)(pnode->item, shablon);
        pnode = pnode->next;
    }
}

void del_last_item(List *plist)
{
    Node *pnode = *plist;

    if (pnode->next == NULL)
    {
        *plist = NULL;
        free(pnode);
        return;
    }

    while (pnode->next->next != NULL)
    {
        pnode = pnode->next;
    }
    free(pnode->next);
    pnode->next = NULL;
}

bool add_item_head(List *plist, Item item)
{
    Node *pnew;

    pnew = (Node *)malloc(sizeof(Node));
    if (pnew == NULL)
    {
        return false;
    }
    else
    {
        copy_to_node(item, pnew);
    }
    pnew->next = *plist;
    *plist = pnew;
    return true;
}

bool del_first_item(List *plist)
{
    if (*plist == NULL)
        return false;

    Node *pnode = *plist;

    *plist = (*plist)->next;
    free(pnode);
    return true;
}

bool del_n_item(List *plist, unsigned int n)
{
    if (*plist == NULL)
        return false;

    Node *pnode = *plist;
    Node *prev = *plist;
    unsigned int i = 1;

    if (n == 1)
    {
        *plist = (*plist)->next;
        free(pnode);
        return true;
    }

    while (i != n && pnode->next != NULL)
    {
        prev = pnode;
        pnode = pnode->next;
        i++;
    }

    if (i < n)
        return false;

    prev->next = pnode->next;
    free(pnode);

    return true;
}

bool add_n_item(List *plist, Item item, unsigned int n)
{
    Node *pnew, *prev;
    Node *pnode = *plist;
    unsigned int i = 0;

    pnew = (Node *)malloc(sizeof(Node));
    if (pnew == NULL)
        return false;

    copy_to_node(item, pnew);

    if (pnode == NULL && n == 0) // list is empty and need to add item to head
    {
        pnode = pnew;
        pnode->next = NULL;
        (*plist) = pnode;
        return true;
    }
    else if (pnode != NULL && n == 0) // list is not empty, add item to head
    {
        *plist = pnew;
        pnew->next = pnode;
        return true;
    }
    else if (pnode == NULL && n > 0) // list is empty, add item not to head -> false
    {
        return false;
    }

    while (i != n && pnode->next != NULL)
    {
        prev = pnode;
        pnode = pnode->next;
        i++;
    }

    if (pnode->next == NULL && i == n - 1) // add item to end
    {
        pnew->next = pnode;
        pnode->next = pnew;
        pnew->next = NULL;
        return true;
    }

    if (i < n)
        return false;

    prev->next = pnew;
    pnew->next = pnode;

    return true;
}