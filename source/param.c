#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "othread.h"
#include <sched.h>

typedef struct _my_thread_arg_t {
  char *thread_name;
  char *string;
  char repetitions;
} my_thread_arg;

void *printer(void *arg)
{
  my_thread_arg *thread_arg = (my_thread_arg *) arg;
  int   i;

  for(i = 0; i<thread_arg->repetitions; i++) {
    printf("%s: %s\n",
	   thread_arg->thread_name,
	   thread_arg->string);
    
    othread_yield();
  }
  
  return NULL;
}

int main(void)
{
  othread_t t1, t2, t3;
  my_thread_arg a1, a2, a3;

  a1.thread_name = "T1";
  a1.string = "I'll be back";
  a1.repetitions = 20;
  othread_create(&t1, printer, (void *) &a1);

  a2.thread_name = "T2";
  a2.string = "Hasta la vista, baby!";
  a2.repetitions = 20;
  othread_create(&t2, printer, (void *) &a2);

  a3.thread_name = "T3";
  a3.string = "I'm back";
  a3.repetitions = 20;
  othread_create(&t3, printer, (void *) &a3);

  othread_join(t1, NULL);

  othread_join(t2, NULL); 

  othread_join(t3, NULL); 

  printf("Main: joined with all threads\n");

  return 0;
}
