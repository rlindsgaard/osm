#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "wqueue.h"

wqueue_t * wq;

void func (void* data)
{
  int x = *(int *) data;
  x++;

  int *i = calloc(1, sizeof(int));
  *i = x;

  if (x % 2 == 0)
    wqueue_ts_insert(wq, x, func, (void *) i);

  sleep(1);

  printf("X is %d", x);
}

void test_main()
{
  printf("Initializing wqueue\n");
  wq = calloc(1, sizeof(wqueue_t));

  wqueue_add(wq,2,3,3);
  wqueue_add(wq,5,8,2);
  wqueue_add(wq,4,3,4);

  printf("Running threads\n");
  int done;
  done = wqueue_thread_pool(wq, 1);  
  if(done==1)
  {
    printf("queue is empty\n");
  }

}
