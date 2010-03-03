#include <stdio.h>
#include "othread.h"

void *thread1(void *arg)
{
  printf("T1: Hi world!\n");

  return NULL;
}
int main(void)
{
  othread_t t1;

  othread_create(&t1, thread1, NULL);
  othread_join(t1, NULL);

  printf("Main: joined with thread 1\n");
  return 0;
}
