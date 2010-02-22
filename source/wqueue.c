#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "wqueue.h"

pthread_cond_t more_work;
pthread_mutex_t more_work_lock;
pthread_mutex_t working_lock;

int threads_working;


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

/*
 * A thread safe version of wqueue_insert().
 * But because it's the pqueue that is thread safe it just calls wqueue_insert().
 */
int wqueue_ts_insert(wqueue_t *wq, unsigned int pri, work_f func, void *data) {
  // insert work
  int result = wqueue_insert(wq, pri, func, data);

  // if successfull insert then send signal
  if (result == 1)
    pthread_cond_signal(&more_work);

  return result;
}

/*
 * 
 */
int wqueue_thread_pool(wqueue_t *wq, int no_threads)
{
  no_threads = 10;
  int threads_working = 0;
  pthread_t thread_ID[10];

  pthread_cond_init(&more_work, NULL);
  pthread_mutex_init(&more_work_lock, NULL);

  // initialize threads
  int i = 0;
  int error_code;
  while (i < no_threads)
  {
    error_code = pthread_create(thread_ID[i], NULL, wqueue_thread, NULL);
    if (error_code) 
      printf("ERROR: return code from pthread_create() is %d\n", error_code);

    i++;
  }

  // clean up
  int i = 0;
  while (i < no_threads)
  {
    pthread_join(thread_ID[i]);
  }

  return 1;
}

void wqueue_thread(void *args)
{
  wqueue_t *wq = args
  int result;

  while (threads_working || wq->e != NULL) 
  {
      pthread_mutex_lock(&working_lock);
      threads_working++;
      pthread_mutex_unlock(&working_lock);

      // run job
      result = wqueue_run(wq);

      pthread_mutex_lock(&working_lock);
      working--;
      pthread_mutex_unlock(&working_lock);

      // wait for signal
      pthread_mutex_lock(&more_work_lock);

      if (result == NULL && threads_working)
        pthread_cond_wait(&more_work, &more_work_lock);

      pthread_mutex_unlock(&more_work_lock);
  }
}


/*
 * Secondary function primary for test use.
 */
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


