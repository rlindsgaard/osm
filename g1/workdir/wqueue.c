#include <stdlib.h>
#include <stdio.h>
#include "wqueue.h"

/*
Indsætter et stykke arbejde i form af en funktion 'func' og det
data 'data' den skal kaldes med. Prioriteten 'pri' angiver
arbejdets prioritet. 'wq' peger på en instans af arbejdskøen.

Lykkedes indsættelsen skal funktionen returnerer '0'. Ved fejl '-1'.
*/
int wqueue_insert(wqueue_t *wq, unsigned int pri, work_f func, void *data) {
  // allocate space for job
  job_t *w;
  w = calloc(1,sizeof(job_t));
  if (w == NULL)
    return -1;
  // create job
  w->func = *func;
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
  job_t *w;
  void *data;
  work_f func;
  
  w = pqueue_remove(wq);
  if (w == NULL)
    return 0;

  // fetch function and data from job
  func = w->func;
  data = w->data;
  
  // run function
  func(data);
  free(w); //the element is of no use to us any longer
  return 1;
}

void print_data(void *s)
{
  printf("Result: %s\n",(char *) s);
}

void wqueue_add(wqueue_t *wq, int a, int b, int pri)
{
  void * s;
  s = calloc(5,sizeof(char));
  sprintf(s,"%d",a+b);
  wqueue_insert(wq,pri,print_data,s);
}


