#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "ring.h"


int main()
{
  //Initializing variables
  pthread_t threads[NUM_THREADS];
  int *baton = calloc(1,sizeof(int));
  int rc;
  *baton = 1;

  //Doing some work
  for(int t=0;t < NUM_THREADS; t++)
  {
    //Fill in the data segment
    data_t *data = calloc(1,sizeof(data_t));
    data->tid = t;
    data->baton = baton;
    data->next = (t+1) % NUM_THREADS;
    //printf("Next is: %d\n",data->next);

    pthread_mutex_init(&locks[t],NULL);
    pthread_mutex_lock(&locks[t]);

    if(t==0)
    {
      rc = pthread_create(&threads[t],NULL,padlock,(void*)data);
    } else {
       rc = pthread_create(&threads[t],NULL,link,(void*)data);
    }
    if(rc)
    {
         printf("ERROR; return code from pthread_create() is %d\n", rc);         
    }
  }
  printf("Initialising %d",0);
  pthread_mutex_unlock(&locks[0]);
  pthread_exit(NULL);
//  pthread_mutex_unlock(&locks[0]);
  
  //Cleaning up
  free(baton);
//  free(thread_data);
//  free(threads);
}

void * link(void * arg)
{
  data_t *data = (data_t*) arg;
  printf("Thread is initialised %d, next is %d\n",data->tid,data->next);
  do {
    printf("Baton is %d\n",*data->baton);
    pthread_mutex_lock(&locks[data->tid]);
    worker(arg);
//    printf("I'm thread number %ld starting for the %d time\n",d.tid,d.count);   
    pthread_mutex_unlock(&locks[data->next]);
  } while(*data->baton);
  printf("Exiting %d\n",data->tid);
  pthread_exit(NULL);
}

void * padlock(void *arg)
{
  data_t *data = (data_t*) arg;
  int count = 0;
  while(count < NUM_RUNS)
  {
    pthread_mutex_lock(&locks[data->tid]);
    printf("I'm thread number %d starting for the %d time. Starting thread %d\n",data->tid,count,data->next);   
    count++;
    pthread_mutex_unlock(&locks[data->next]);
  }
  *data->baton = 0;
  pthread_mutex_unlock(&locks[data->next]);
  pthread_exit(NULL);
}

void * worker(void * arg)
{
  data_t *data = (data_t*) arg;
  printf("Doing some work for thread: %d, the baton is %d\n",(int) data->tid,(int) *data->baton);
  for(int i=0;i<100;i++){}
  //sleep(1);
}
