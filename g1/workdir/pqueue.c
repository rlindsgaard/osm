
struct entry {
  int pri;
  entry_t *next;
  void *elem;
};

int pqueue_insert(pqueue_t *pq, unsigned int pri, void *elem)
{
  entry_t *previous;
  entry_t *current;
  if(pq == NULL)
    return -1;
  current = pq->e;

  //Create new entry
  entry_t *new_elem;
  new_elem = malloc(sizeof(entry_t));
  if(new_elem == NULL)  //assert that we get allocated memory
    return -1; 
  new_elem->pri = pri;
  new_elem->elem = elem;
  new_elem->next = NULL;

  previous = NULL;
  //Run through the sorted list
  while(current != NULL && pri >= current->pri)
  {
    previous = current;
    current = current->next;
  }
  new_elem->next = current;
  
  if(previous != NULL)
  {
    previous->next = new_elem;
  } else {
    pq->e = new_elem;
  }
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
  if(pq == NULL || pq->e == NULL)
    return -1;

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
