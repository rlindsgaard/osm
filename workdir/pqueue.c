#include <stdio.h>
#include <pqueue.h>

struct entry {
  int pri;
  void *elem;
  int next;
} entry_t;

int pqueue_insert(pqueue_t *pq, unsigned int pri, void *elem)
{
  entry *previous;
  entry *current;
  current = pq->e;

  //Create new entry
  entry *new_elem;
  new_elem = malloc(sizeof(entry));
  if(new_elem == NULL)  //assert that we get allocated memory
    return -1; 
  new_elem.pri = pri;
  new_elem.elem = elem;
  new_elem.next = null;

  //List is empty
  if(current == NULL)
  {
    pq->e = new_entry;
    return 0;
  }

  //Run through the sorted list
  while(current != NULL && pri <= current->pri)
  {
    previous = current;
    current = current->next;
  }
  new_elem.next = current;
  previous.next = new_elem;
  return 0;
}

void *pqueue_remove(pqueue_t *pq)
{
  /*
   * 1. Copy the entry value
   * 2. Update the pqueue
   * 3. Free memory from the late first entry
   * 4. Return the pointer to the element
   * 5. Profit
   */
  void t_elem;
  void *t_entry;
  t_entry = pq->e;
  t_elem = t_entry.elem;
  pq->e = &t_entry->next;

  free(t_entry);

  return t_elem;
}
