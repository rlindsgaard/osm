#ifndef _DLINK_H
#define _DLINK_H

typedef struct dlink {
  struct dlink *prev;
  struct dlink *next;
  void *data;
} dlink_t;

typedef struct dlink_head {
  dlink_t *first;
  dlink_t *last;
} dlink_head_t;

#define DLINK_HEAD_INIT {NULL, NULL}

/* Initialises the head variable to all NULL pointers. */
void dlink_init_head(dlink_head_t *head);

/* Returns true if the link is empty. */
char dlink_empty(dlink_head_t *head);

/* Returns a pointer to a newly allocated element for our doubly
   linked list. data is used to initialise the data field of the
   element. */
dlink_t *dlink_alloc(void *data);

/* Deallocates the memory allocated for the linked list metadata. If
   the data field is not NULL, the deallocation fails. */
int dlink_free(dlink_t *elem);

/* Inserts an element at the beginning of the list pointed to by
   head. If head is NULL, the operation fails. */
int dlink_insert(dlink_head_t *head, dlink_t *elem);

/* Removes the last element from the list pointed to by head. If head
   is NULL, NULL is returned. */
dlink_t *dlink_remove(dlink_head_t *head);

/* Removes the (first) element that contains the data pointer data
   from the list. If head is NULL or if the data pointer isn't found,
   NULL is returned. */
dlink_t *dlink_delete(dlink_head_t *head, void *data);

#endif
