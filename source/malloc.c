#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "othread.h"
#include <sched.h>

typedef struct counter_t {
  int count;
} counter;

typedef struct _my_thread_arg_t {
  char *thread_name;
  char *string;
  char repetitions;
  counter *counter;
} my_thread_arg;

void *printer(void *arg)
{
  my_thread_arg *thread_arg = (my_thread_arg *) arg;
  int   i;
  // A shared counter
  int * c = othread_malloc(sizeof(int),1);
  // A private counter only for this thread
  int * p = othread_malloc(sizeof(int),0);
  //(*c) = 0; Assume this is already set because it is initialized to NULL

  for(i = 0; i<thread_arg->repetitions; i++) {
    (*c)++;
    (*p)++;
    printf("%s says: %s\nPublic counter is: %d\nPrivate counter is: %d\n",
	   thread_arg->thread_name,
	   thread_arg->string,
     *c,*p);
    othread_yield();
  }
  int *d = othread_malloc(sizeof(int),2);
  printf("Value of the other thread's global counter: %d\n",*d);
  return NULL;
}

void* printer2(void *arg)
{
  int i;
  my_thread_arg *thread_arg = (my_thread_arg *) arg;

  int *c = othread_malloc(sizeof(int),1);
  int *d = othread_malloc(sizeof(int),2); //A second shared variable working as private
  int *p = othread_malloc(sizeof(int),0);

  for(i = 0;i<thread_arg->repetitions;i++)
  {
    (*c)++;
    (*p)++;
    (*d)++;
    printf("%s says: %s\nPublic counter is: %d\nPrivate counter is: %d\n",
	   thread_arg->thread_name,
	   thread_arg->string,
     *c,*p);
    printf("This thread also has a global counter: %d\n",*d);
    othread_yield();
  }
  printf("Exiting printer2, memid=2 will no longer be readable\n");
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
  othread_create(&t2, printer2, (void *) &a2);
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
