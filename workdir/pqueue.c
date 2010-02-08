
struct entry {
  int pri;
  void *elem;
  entry_t *next;
};

int pqueue_insert(pqueue_t *pq, unsigned int pri, void *elem)
{
  entry_t *previous;
  entry_t *current;
  current = pq->e;

  //Create new entry
  entry_t *new_elem;
  printf("allocation new_elem\n");
  new_elem = malloc(sizeof(entry_t));
  if(new_elem == NULL)  //assert that we get allocated memory
    return -1; 
  new_elem->pri = pri;
  new_elem->elem = elem;
  new_elem->next = NULL;

  //List is empty
  
  if(current == NULL)
  {
    printf("sizeof list=0\n");
    pq->e = new_elem;
    return 0;
  }
  printf("we're gonna fail\n");
  //Run through the sorted list
  while(current != NULL && pri <= current->pri)
  {
    previous = current;
    current = current->next;
  }
  printf("we're already screwed");
  new_elem->next = current;
  printf("previous: %p\n",previous);
  if(previous != NULL)
    previous->next = new_elem;
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
  void *t_elem;
  entry_t *t_entry;
  t_entry = pq->e;
  t_elem = t_entry->elem;
  pq->e = t_entry->next;

  free(t_entry);

  return t_elem;
}

void pqueue_print(pqueue_t *pq)
{
  entry_t *current;
  current = pq->e;
  //printing queue
  while(current != NULL)
  {
    printf("Priority: %d Elem: %p\n",current->pri,current->elem);
    current = current->next;
  }

}
