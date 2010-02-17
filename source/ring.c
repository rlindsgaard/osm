#include <pthread.h>
#include <stdio.h>
#define NUM_THREADS     5

int i =0;
pthread_mutex_t mutexsum;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;


void *PrintHello(void *threadid)
{
   long tid;
   pthread_mutex_lock(&count_mutex);
   
   tid = (long)threadid;
   int r = i % NUM_THREADS;
   while(tid == r) {  
     pthread_cond_wait(&count_threshold_cv, &count_mutex) ;
   
       printf("Hello World! It's me, thread #%ld!\n %d\n", tid,i);
     i++;
     pthread_cond_signal(&count_threshold_cv);
   }
   pthread_mutex_unlock(&count_mutex);
    pthread_exit(NULL);

}

int main (int argc, char *argv[])
{
  /* Initialize mutex and condition variable objects */
  pthread_mutex_init(&count_mutex, NULL);
  pthread_cond_init (&count_threshold_cv, NULL);

   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   for(t=0; t<NUM_THREADS; t++){
      printf("In main: creating thread %ld\n", t);
      rc = pthread_create(&threads[t], NULL, PrintHello, (void *)t);
      if (rc){
         printf("ERROR; return code from pthread_create() is %d\n", rc);
         
      }
   }
   pthread_exit(NULL);
   i=0;
}
