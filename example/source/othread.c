#include <sched.h>
#include <time.h>
#include <ucontext.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "othread.h"
#include "dlink.h"

/* Type definitions */

#define STACK_SIZE 1024 /* Stacksize in integers - size in bytes is
			   sizeof(int)*STACK_SIZE */

/* The states refer to the following:
   INIT: the thread is created but the main thread hasn't been
         activated
   READY: the thread has started executing and is now waiting for the
          processor
   RUNNING: the thread is the current active thread
   WAITING: the thread is waiting for an event to occur, e.g., unlock
            of mutex
   DONE: the thread has been terminated and is waiting for somebody
         to join with it. */

typedef enum thread_state {INIT, READY, RUNNING, WAITING, DONE} state_t;

/* We need to register the threads main function, it's parameters, a
   return value, the context, a state (related to the lifecycle of
   the thread, a wait queue for a joining thread and a stack. */
struct tkb {
  void *     (*start_routine) (void *);
  void        *arg;
  void        *retval;
  ucontext_t   context;
  state_t      state;
  dlink_head_t waiting;
  int          stak[STACK_SIZE];
};

/* We need a TKB for the first thread. We don't need to initialize it
   as the thread already has a stack and everything. Like everybody
   else, it will have its context saved when calling
   swapcontext. However, we must be careful not to free the memory of
   the first_thread TKB. */

static struct tkb first_thread;

struct tkb *current = &first_thread; /* The current active thread */
dlink_head_t ready = {NULL, NULL}; /* The ready queue */
dlink_head_t termineret = {NULL, NULL}; /* This isn't really necessary
					   currently, but it is nice
					   to have for debugging and
					   statistics. */


typedef void (*arg_fn)(void); /* This is just to make typecasting of
				 thread_wrapper easier */

/* This is an auxiliary function which takes care of starting a new
   thread and picking up its return value, if necessary. */
void thread_wrapper(struct tkb *my_tkb)
{
  void *retval;

  my_tkb->state = RUNNING;

  /* We activate the main function of the thread. If the thread
     terminates through a return call, we will catch it
     here. Otherwise, we never return to this function. */
  retval = my_tkb->start_routine(my_tkb->arg);

  /* We need to call the exit routine on behalf of our thread. */
  othread_exit(retval);
} 

/* Encapsulates switching between two threads - the current thread is
   placed in the FIFO old and the new current is picked from the FIFO
   new. The state of the old thread is changed to the state indicated
   by the state argument.

   Att: This routines assumes that if there are no threads in the new
   queue, we are done executing the application. */
void switch_thread(dlink_head_t *old, dlink_head_t *new, int state)
{
  if(!dlink_empty(new)) {
    printf("Ping %d\n",current);
    /* If there is at least one thread waiting in the new queue, we
       switch to the first one. Otherwise we terminate the whole
       application */
      dlink_t *d;
      current->state = state;
      struct tkb * old_current = current;
      
      d = dlink_alloc(old_current);
      dlink_insert(old,d);
      
      dlink_t *new_d;
      new_d = dlink_remove(new);
    
      current = new_d->data;
//      free(new_d);
        current->waiting = *old;
    //    printf("swapcontext");
      printf("old_current: %p current: %p\n",old_current,current);
      swapcontext(&old_current->context,&current->context);
   
   /* TO IMPLEMENT: The actual switch between the current thread and
       the first thread at the head of the "new" list. Here you should
       use swapcontext as explained in the note on user level thread
       switching. The current thread is place in the "old" queue, and
       it's state has to be updated to the "state" parameter. */

  } else {
    printf("Terminating: empty new FIFO in switch_thread\n");
    exit(-1);
  }
}

/* Function for handling threads.  */

int othread_create (othread_t *threadp,
                    void *(*start_routine) (void *), void *arg)
{
  struct tkb *tkbp; /* The control block for the new thread. */
  dlink_t *new;

  /* First we allocate a control block for the new thread. */
  tkbp = (struct tkb *) malloc(sizeof(struct tkb));
  if(tkbp == NULL) {
    /* Error - not enough memory for the TKB */
    printf("Out of memory: couldn't allocate TKB\n");
    exit(-1);
  }
  /* We save the main routine and its argument for later activation */
  tkbp->start_routine = start_routine;
  tkbp->arg = arg;
  if(getcontext(&tkbp->context) != 0) {
    printf("Error in getcontext\n");
    exit(-1);
  }
  /* Set up stack for the new proces and store reference and size in
     the context structure. When using GNU lib C¸ ss_sp should point
     to the beginning of the memory region containing the stack - even
     though the x86 stack grows downwards. */
  tkbp->context.uc_stack.ss_sp = tkbp->stak;
  tkbp->context.uc_stack.ss_size = STACK_SIZE*sizeof(int);
  tkbp->context.uc_stack.ss_flags = 0;
  tkbp->context.uc_link = NULL;
  /* We use the thread wrapper to start threads so that we later can
     get the return value of a thread if it exits by return. */
  makecontext(&tkbp->context, (arg_fn) thread_wrapper, 1, tkbp);

  tkbp->state = INIT;
  tkbp->retval = NULL;
  dlink_init_head(&tkbp->waiting);

  /* Save reference to the new thread in the active queue. */
  if((new = dlink_alloc(tkbp)) == NULL) {
    printf("Out of memory: couldn't allocate dlink structure\n");
    exit(-1);
  }
  dlink_insert(&ready, new); /* Insert created in ready queue */

  /* We use the tkb pointer as thread ID */
  *threadp = (int) tkbp;

  return 0;
}

/* Terminate calling thread. Never returns :-) */
void othread_exit (void *retval)
{
  current->retval = retval;

  if(!dlink_empty(&current->waiting)) {
    dlink_t * d;
    d= dlink_remove(&current->waiting);
    printf("Adding new element when exiting %p\n",d->data);
    dlink_insert(&ready,d );
  }
  /* Let othread_join handle the actual deallocation after we've
     switched to a new context - right now we are still using the
     stack. Postponing deallocation ensures that the thread is indeed
     done, when we deallocate the stack and tkb. */
  switch_thread(&termineret, &ready, DONE);
}

/* Make calling thread wait for termination of the thread TH.  The
   exit status of the thread is stored in *THREAD_RETURN, if THREAD_RETURN
   is not NULL.
   Return codes:
    0       succes
    ESRCH   No thread could be found corresponding to that specified by th.
    EINVAL  Another thread is already waiting on termination of th.
    EDEADLK The th argument refers to the calling thread. */
int othread_join (othread_t th, void **thread_return)
{
  struct tkb *wait_for = (struct tkb *) th;
  dlink_t *elem;

  if(wait_for == current)
    return EDEADLK;
  if(wait_for == NULL)
    return ESRCH;
  /* If another thread is waiting, it's an error */
  if(!dlink_empty(&wait_for->waiting))
    return EINVAL;
  /* If the thread hasn't terminated we wait for it to do so */
  if(wait_for->state != DONE)
    switch_thread(&wait_for->waiting, &ready, WAITING);
  /* We resume here when the thread has terminated */
  if(thread_return != NULL)
    *thread_return = wait_for->retval;
  elem = dlink_delete(&termineret, wait_for);
  /* We shouldn't free the statically allocated first_thread tkb */
  if(wait_for != &first_thread)
    free(wait_for);
  if(elem != NULL) {
    elem->data = NULL;
    dlink_free(elem);
  }
    
  return 0;
}

/* Yield the processor to another thread or process. Returns 0 always. */
int othread_yield (void)
{
  /* TO IMPLEMENT: If there are other threads ready to run on the
     ready queue, switch between the current thread and the first one
     in the ready queue. The current thread should be placed in the
     ready queue. */
  if(!dlink_empty(&ready))
  {
    printf("Adding a new element %p\n",current);
    dlink_insert(&ready,dlink_alloc(current));
    
 //   print_ready(&ready);
    switch_thread(&current->waiting,&ready,READY);
  }
  
  return 0;
}

/* Functions for mutex handling.  */

/* Wait until lock for MUTEX becomes available and lock it.
   Return codes:
    0        succes
    EINVAL   the mutex has not been properly initialized.
    EDEADLK  the  mutex  is  already  locked  by  the
             calling  thread (``error checking'' mutexes
             only).                                          */
int othread_mutex_lock (othread_mutex_t *mutex)
{
  if(mutex == NULL)
    return EINVAL;

  if(mutex->locked)
  {
    current->state = WAITING;
    dlink_insert(mutex->waiting,dlink_alloc(current));

    //othread_yield();

    //The mutex is locked
    //Put it on the waiting queue
    //Get new active thread from ready queue
  } else {
    //The mutex is not locked
    //Lock the mutex
    mutex->locked = 1;
  }
  
  /* TO IMPLEMENT: Check whether the mutex is locked. If not lock it,
     and otherwise put the thread on the mutex wait queue and get a
     new active thread from the ready queue.*/

  return 0;
}

/* Unlock MUTEX.                                        
   Return codes:                                        
    0      succes
    EINVAL the mutex has not been properly initialized. 
    EPERM  the calling thread does not own the mutex    
          (``error checking'' mutexes only).            */
int othread_mutex_unlock (othread_mutex_t *mutex)
{
  if(mutex == NULL)
    return EINVAL;

  if(dlink_empty(mutex->waiting))
  {
    mutex->locked = 0; //Unlock the mutex
  } else {
    dlink_t * elem;
    elem = dlink_remove(mutex->waiting);
    dlink_insert(&ready,elem);
  }
  /* TO IMPLEMENT: If there are no waiting threads the mutex should be
     unlocked. Otherwise, one of the waiting threads should be moved
     to thre ready queue. */
  
  return 0;
}

void print_ready(dlink_head_t ready)
{
  dlink_t *d;

  printf("Printing list");
  while(!dlink_empty(&ready))
  {
    d = dlink_remove(&ready);
    printf("Ready element: %p\n",d->data);
  //  free(d);
  }
}
