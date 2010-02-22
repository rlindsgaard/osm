#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "ring.h"


int _main()
{

  //Initializing variables
  pthread_t *threads = calloc(NUM_THREADS,sizeof(pthread_t));
  data_t *data = calloc(NUM_THREADS,sizeof(data_t));
  int rc;
  int t;

  baton_t *baton = calloc(1,sizeof(baton_t));
  baton->current = 0;
  baton->counter = 0;
  baton->stop = 0;
  pthread_mutex_init(&baton->mutex,NULL);
  pthread_mutex_lock(&baton->mutex);
  baton->somedata = (void *) "This is a string representing data in the baton";

  //Doing some work
  for(t=0;t < NUM_THREADS; t++)
  {
    //Fill in the data segment
    data[t].tid = t;
    data[t].baton = baton;
    pthread_cond_init(&data[t].cond,NULL); 
    data[t].next = (t+1 == NUM_THREADS ? data : data+t+1);
    printf("%p\n",(void *)data[t].next);
    printf("Initialising thread");
    rc = pthread_create(&threads[t],NULL,link,(void*) (data+t));
    
    if(rc)
    {
         printf("ERROR; return code from pthread_create() is %d\n", rc);         
    }
  }
  printf("Initialising %d\n",0);
  pthread_mutex_unlock(&baton->mutex);
  for(int i=0;i < NUM_THREADS; i++)
  {
    pthread_join(threads[i],NULL);
  }
  
  //Cleaning up
  free(baton);
  free(threads);
  free(data);
}

void * link(void * arg)
{
  int stop = 0;
  data_t *data = (data_t*) arg;
  baton_t *baton = data->baton;
  while(!stop){
    pthread_mutex_lock(&baton->mutex);
    
    while(data->tid != baton->current)
    {
      pthread_cond_signal(&data->next->cond);
      pthread_cond_wait(&data->cond,&baton->mutex);
    }
    
    baton->current = data->next->tid;
    pthread_cond_signal(&data->next->cond);  
    
    
    if(data->tid+1 == NUM_THREADS)
      baton->counter++;

   if(baton->counter > NUM_RUNS)
      baton->stop = 1;
    stop = baton->stop;
   //Do work

    worker(arg);
   
    pthread_mutex_unlock(&baton->mutex);
    
  }
  pthread_cond_signal(&data->next->cond);
  printf("Exiting %d\n",data->tid);
  
  return NULL;
}

void * worker(void * arg)
{
  data_t *data = (data_t*) arg;
  printf("Doing some work for thread: %d, the baton is %d\n",(int) data->tid,!(int) data->baton->stop);
  for(int i=0;i<100;i++){}
  return NULL;
}
