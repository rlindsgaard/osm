#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "wqueue.h"

pthread_cond_t more_work;       // is there more work?
pthread_mutex_t more_work_lock; // lock when listening for more work
pthread_mutex_t working_lock;   // lock when updating the threads_working variable

int threads_working;            // number of threads working right now

// argument to a thread
struct thread_args {
  wqueue_t *wq;
  int id;
};


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
 * Starts a pool of treads to run the jobs in the wqueue.
 */
int wqueue_thread_pool(wqueue_t *wq, int no_threads)
{
  no_threads = 10;         // number of threads to start
  threads_working = 0;     // number of threads currently working
  pthread_t thread_ID[10]; // array of threat id's

  // init locks
  pthread_cond_init(&more_work, NULL);
  pthread_mutex_init(&more_work_lock, NULL);

  // initialize threads
  int i = 0;
  int error_code;
  printf("\tInitializing threads\n");
  while (i < no_threads)
  {
    printf("\t\tCreating thread with id: %d\n", i); 
    thread_args_t *args = calloc(1,sizeof(thread_args_t));
    args->wq = wq;
    args->id = i;

    error_code = pthread_create(&thread_ID[i], NULL, wqueue_thread, args);
    if (error_code)
      printf("\t\tERROR: return code from pthread_create() is %d\n", error_code);

    i++;
  }

  // wait for threads to terminate
  printf("\tJoining threads\n");
  i = 0;
  while (i < no_threads)
  { 
    printf("\t\tJoining thread with id: %d\n", i); 
    pthread_cond_signal(&more_work);
    pthread_join(thread_ID[i], NULL);
    i++;
  }

  return 1;
}

/*
 * Function each threads is running.
 */
void* wqueue_thread(void *args)
{
  // fetch values from thread argument
  thread_args_t *thread_args = args;
  wqueue_t *wq = thread_args->wq;
  int id = thread_args->id;

  int result;

  printf("\t\t\tThread %d starting up\n", id);
  while (threads_working || wq->e != NULL) 
  {
      // lock working mutex, update counter and unlock again
      pthread_mutex_lock(&working_lock);
      threads_working++;
      pthread_mutex_unlock(&working_lock);

      // run job
      result = wqueue_run(wq);

      // again, lock working mutex, update counter and unlock
      pthread_mutex_lock(&working_lock);
      threads_working--;
      pthread_mutex_unlock(&working_lock);

      // check if there is more work to be done
      printf("\t\t\tThread %d checking for more work\n", id);
      pthread_mutex_lock(&more_work_lock);

      while(result == 0 && threads_working) { // if the queueu was empty but other threads are working
        printf("\t\t\tThread %d sleeping\n", id);
        pthread_cond_wait(&more_work, &more_work_lock); // wait for job to be inserted
      }

      pthread_mutex_unlock(&more_work_lock);
      printf("\t\t\tThread %d requesting more work\n", id);
  }

  printf("\t\t\tThread %d shutting down\n", id);
  pthread_cond_signal(&more_work); // before termination; send signal prevent deadlocks
  return NULL;
}


/*
 * Secondary function primary for test use.
 */


void print_data(void *s)
{
  printf("Result: %s\n",(char *) s);
  sleep(1);
}

void wqueue_add(wqueue_t *wq, int a, int b, int pri)
{
  void * s;
  s = calloc(5,sizeof(char));
  sprintf(s,"%d",a+b);
  
  int o=6;
  while(o!=0)
  {
  wqueue_ts_insert(wq,pri,print_data,s);
  o--;
  }
}


