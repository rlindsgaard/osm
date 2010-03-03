#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "othread.h"

void *thread1(void *arg)
{
  int   i;

  for(i = 0; i<10; i++) {
    printf("T1: Hi world!\n");

    othread_yield();
  }
  
  return NULL;
}

void *thread2(void *arg)
{
  char *arg_string = (char *) arg;
  char *return_string;
  int   i;

  for(i = 0; i<10; i++) {
    printf("T2: %s\n", arg_string);

    othread_yield();
  }
  
  return_string = strdup("Godbye cruel world!");

  othread_exit(return_string);

  return 0;
}

int main(void)
{
  othread_t t1, t2;
  char *return_val;

  othread_create(&t1, thread1, NULL);

  othread_create(&t2, thread2, "Hello world!");

  othread_join(t1, NULL);

  othread_join(t2, (void **) &return_val); 

  printf("Main: thread2 returned %s\n", return_val);

  return 0;
}
