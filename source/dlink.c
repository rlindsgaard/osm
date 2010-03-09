#include <stdlib.h>
#include <stdio.h>
#include "dlink.h"

void dlink_init_head(dlink_head_t *head)
{
  if(head != NULL) {
    head->first = NULL;
    head->last = NULL;
  }
}

char dlink_empty(dlink_head_t *head)
{
  return(head->first == NULL);
}

dlink_t *dlink_alloc(void *data)
{
  dlink_t *new;

  new = malloc(sizeof(dlink_t));
  if(new != NULL) {
    new->prev = NULL;
    new->next = NULL;
    new->data = data;
  }
  
  return new;
}


int dlink_free(dlink_t *elem)
{
  if(elem != NULL && elem->data != NULL) {
    free(elem);
    return 0;
  } else
    return -1;
}

int dlink_insert(dlink_head_t *head, dlink_t *elem)
{
  if(head==NULL || elem==NULL) {
    return -1;
  }
  
  if(head->first == NULL) {
    head->first = head->last = elem;
    elem->prev = NULL;
    elem->next = NULL;    
  } else {
    head->first->prev = elem;
    elem->next = head->first;
    head->first = elem;
  }

  return 0;
}

dlink_t *dlink_remove(dlink_head_t *head)
{
  dlink_t *elem;

  if(head==NULL || head->last == NULL)
    return NULL;

  elem = head->last;
  if(head->last == head->first)
    head->last = head->first = NULL;
  else {
    head->last = head->last->prev;
    head->last->next = NULL;
  }
  elem->prev = NULL;

  return elem;
}

dlink_t *dlink_delete(dlink_head_t *head, void *data)
{
  dlink_t *elem;

  if(head==NULL)
    return NULL;

  elem = head->first;
  while(elem != NULL && elem->data != data)
    elem = elem->next;
  if(elem != NULL) {
    if(elem == head->first) {
      head->first = head->first->next;
      if(head->first != NULL)
	head->first->prev = NULL;
    } else if(elem != head->last) {
      elem->prev->next = elem->next;
    }
    if(elem == head->last) {
      head->last = head->last->prev;
      if(head->last != NULL)
	head->last->next = NULL;
    } else {
      elem->next->prev = elem->prev;
    }
  }

  return elem;
}
