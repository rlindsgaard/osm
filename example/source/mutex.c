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

othread_mutex_t global_mutex = OTHREAD_NORMAL_MUTEX_INITIALIZER;

void *printer(void *arg)
{
  my_thread_arg *thread_arg = (my_thread_arg *) arg;
  int   i;

  othread_mutex_lock(&global_mutex);
  for(i = 0; i<thread_arg->repetitions; i++) {
    printf("%s: %s\n",
	   thread_arg->thread_name,
	   thread_arg->string);
    
    othread_yield();
  }
  othread_mutex_unlock(&global_mutex);
  
  return NULL;
}

int main(void)
{
  othread_t t1, t2, t3;
  my_thread_arg a1, a2, a3;

  a1.thread_name = "T1";
  a1.string = "I'll be back";
  a1.repetitions = 10;
  othread_create(&t1, printer, (void *) &a1);

  a2.thread_name = "T2";
  a2.string = "Hasta la vista, baby!";
  a2.repetitions = 3;
  othread_create(&t2, printer, (void *) &a2);

  a3.thread_name = "T3";
  a3.string = "I'm back";
  a3.repetitions = 3;
  othread_create(&t3, printer, (void *) &a3);

  othread_join(t1, NULL);

  othread_join(t2, NULL); 

  othread_join(t3, NULL); 

  printf("Main: joined with all threads\n");

  return 0;
}
