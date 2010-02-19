#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS     5

int i =0;
pthread_mutex_t count_mutex[NUM_THREADS];
int baton = 1;


void * crown(void *threadid){
  long tid;
  tid = (long)threadid;
  int count = 0;
  int r = (tid + 1)% NUM_THREADS;
  while(count < 5)
  {
       pthread_mutex_lock(&count_mutex[tid]);
       printf("I'm thread number %ld starting for the %d time\n",tid,count);   
       pthread_mutex_unlock(&count_mutex[r]);
       count++;
  }
  baton = 0;
  pthread_mutex_unlock(&count_mutex[r]);
   pthread_exit(NULL);
}


void *PrintHello(void *threadid){
   long tid;
   int r;
   tid = (long)threadid;
   r = (tid +1) % NUM_THREADS;

   while(baton){
       pthread_mutex_lock(&count_mutex[tid]);
       printf("I'm thread number %ld sending to thread %d and baton is %d\n",tid,r,baton);   
       pthread_mutex_unlock(&count_mutex[r]);
       //sleep(1);
   }
   
   pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
  /* Initialize mutex and condition variable objects */



   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t<NUM_THREADS; t++){
     pthread_mutex_init(&count_mutex[t],NULL);
   pthread_mutex_lock(&count_mutex[t]);
      printf("In main: creating thread %ld\n", t);
      if(t==0)
         rc = pthread_create(&threads[t], NULL, crown, (void *)t);
      else
	rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);         
      }
   }
   pthread_mutex_unlock(&count_mutex[0]);
   pthread_exit(NULL);
    i=0;
}
