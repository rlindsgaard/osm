#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "wqueue.h"

pthread_mutex_t pqueue_lock;

pthread_mutex_init(&pqueue_lock, NULL);

struct entry {
  int pri;
  entry_t * next;
  void *elem;
};

int pqueue_insert(pqueue_t *pq, unsigned int pri, void *elem)
{
  // create new entry
  entry_t *new_elem;
  new_elem = calloc(1,sizeof(entry_t));
  if(new_elem == NULL)  // assert that we get allocated memory
    return -1; 

  new_elem->pri = pri;
  new_elem->elem = elem;
  new_elem->next = NULL;

  // scan through the sorted list
  entry_t *previous;
  entry_t *current;

  pthread_mutex_lock(&pqueue_lock);

  previous = NULL;
  current = pq->e;
  while(current != NULL && pri >= current->pri)
  {
    previous = current;
    current = current->next;
  }

  // insert element into list
  new_elem->next = current;
  
  if(previous != NULL)
  {
    previous->next = new_elem;
  } else {
    pq->e = new_elem;
  }
  pthread_mutex_unlock(&pqueue_lock);

  return 0;
}

void *pqueue_remove(pqueue_t *pq)
{
  // empty list?
  if(pq->e == NULL)
    return NULL;

  void *t_elem;
  entry_t *t_entry;

  // take out pqueue head
  pthread_mutex_lock(&pqueue_lock);
  t_entry = pq->e;

  // set pqueue head to next
  pq->e = t_entry->next;
  pthread_mutex_unlock(&pqueue_lock);
  
  // fetch data
  t_elem = t_entry->elem;
  free(t_entry);

  return t_elem;
}

void pqueue_print(pqueue_t *pq)
{
  entry_t *current;
  current = pq->e;
  //printing queue
  printf("\nPrinting list\n");
  while(current != NULL)
  {
    print_struct(current,69);
    current = current->next;
  }

}

void print_struct(entry_t *p,int line)
{
  int element;
  if(p != NULL)
  {
    element = *((int *) p->elem);
    printf("%d Struct pri: %d elem: %d next:%p\n",line,p->pri,element,(void *) p->next);
  } else {
    printf("%d Struct is NULL\n",line);
  }
}
