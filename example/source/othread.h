/* Osmthreads - en forsimplet udgave af Posix traadbiblioteket i Linux   */
/* Dette er en modificeret udgave af headerfilen normalt placeret i      */
/* /usr/include/pthreads som er copyright (C) 1996 Xavier Leroy          */
/* (Xavier.Leroy@inria.fr). Se den fil for naermere bestemmelser omkring */
/* brugen af pthread.h                                                   */

#ifndef _OTHREAD_H
#define _OTHREAD_H	1

#include <sched.h>
#include <time.h>
#include "dlink.h"

/* Type definitions */

/* othread_t */
typedef unsigned long int othread_t;

/* othread_mutex_t */
typedef struct {

  int locked; //0 if unlocked, 1 if locked
  dlink_head_t waiting;

} othread_mutex_t;


/* Initializers.  */

#define OTHREAD_NORMAL_MUTEX_INITIALIZER \
  { /* TO IMPLEMENT */ }

/* Function for handling threads.  */

/* Create a thread, and call function START_ROUTINE with given
   arguments ARG.
   Return codes:
    0       succes
    EAGAIN  Not enough system resources were available to create thread */
int othread_create (othread_t *threadp,
                    void *(*start_routine) (void *), void *arg);

/* Terminate calling thread. Never returns :-) */
void othread_exit (void *retval);

/* Make calling thread wait for termination of the thread TH.  The
   exit status of the thread is stored in *THREAD_RETURN, if THREAD_RETURN
   is not NULL.
   Return codes:
    0       succes
    ESRCH   No thread could be found corresponding to that specified by th.
    EINVAL  Another thread is already waiting on termination of th.
    EDEADLK The th argument refers to the calling thread. */
int othread_join (othread_t th, void **thread_return);

/* Yield the processor to another thread or process. Returns 0 always. */
int othread_yield (void);

/* Functions for mutex handling.  */

/* Wait until lock for MUTEX becomes available and lock it.
   Return codes:
    0        succes
    EINVAL   the mutex has not been properly initialized.
    EDEADLK  the  mutex  is  already  locked  by  the
             calling  thread (``error checking'' mutexes
             only).                                          */
int othread_mutex_lock (othread_mutex_t *mutex);

/* Unlock MUTEX.                                        
   Return codes:                                        
    0      succes
    EINVAL the mutex has not been properly initialized. 
    EPERM  the calling thread does not own the mutex    
          (``error checking'' mutexes only).            */
int othread_mutex_unlock (othread_mutex_t *mutex);

#endif	/* othread.h */
