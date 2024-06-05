#include <malloc.h>
#include <stdbool.h>
#include "double_linked_list.h"

DblLinkedList* initDblLinkedList() 
{
    DblLinkedList *tmp = (DblLinkedList*) malloc(sizeof(DblLinkedList));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;
 
    return tmp;
}

void pushBack(DblLinkedList *list, int value) 
{
    list_node *tmp = (list_node*) malloc(sizeof(list_node));

    tmp->val = value;
    tmp->next = NULL;
    tmp->prev = list->tail;
    if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;
 
    if (list->head == NULL) {
        list->head = tmp;
    }
    list->size++;
}

void deletElem(list_node *lst)
{
  list_node *prev, *next;
  prev = lst->prev;
  next = lst->next;
  if (prev)
    prev->next = NULL; 
  if (next)
    next->prev = NULL; 
  free(lst); 
}
