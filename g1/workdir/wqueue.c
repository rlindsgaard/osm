#include "wqueue.h"
#include <stdlib.h>
#include <stdio.h>

/*
Indsætter et stykke arbejde i form af en funktion 'func' og det
data 'data' den skal kaldes med. Prioriteten 'pri' angiver
arbejdets prioritet. 'wq' peger på en instans af arbejdskøen.

Lykkedes indsættelsen skal funktionen returnerer '0'. Ved fejl '-1'.
*/
int wqueue_insert(wqueue_t *wq, unsigned int pri, work_f func, void *data) {
  // allocate space for job
  job_t *w = calloc(1,sizeof(job_t));
  if (w == NULL)
    return -1;
  
  // create job
  w->func = func;
  w->data = data;
  
  // insert job into pqueue
  return pqueue_insert(wq, pri, w);
}

/*
Udfører et stykke arbejde fra køen.

Hvis køen er tom returneres '0', ellers '1'.
*/
int wqueue_run(wqueue_t *wq) {
  // get job from pqueue
  job_t *w = pqueue_remove(wq);

  if (w == NULL)
    return 0;
  
  // fetch function and data from job
  work_f func = w->func;
  void *data = w->data;

  // run function
  func(data);
  printf("wqueue_run\n");
  
  return 1;
}

void print_data(void *s)
{

  printf("%s\n",(char *) s);
}

void wqueue_test(wqueue_t *wq, int a, int b, int pri)
{
  char s[5];
  int val;
  val = a+b;
  sprintf(s,"%d",val);
//  printf("wqueue_test %\n",&print_data);
  wqueue_insert(wq,pri,*print_data,s);
}


