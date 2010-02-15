#include <pqueue.c>

#define WQUEUE_INIT PQUEUE_INIT

/*
Definition af arbejdsfunktionen.
*/
typedef void (*work_f)(void *);

/*
wqueue_t er simpelthen bare en prioritetskø, som indeholder
arbejdsopgaver.
*/
typedef pqueue_t wqueue_t;

struct job {
    work_f *func;
    void   *data;
};

/*
Indsætter et stykke arbejde i form af en funktion 'func' og det
data 'data' den skal kaldes med. Prioriteten 'pri' angiver
arbejdets prioritet. 'wq' peger på en instans af arbejdskøen.

Lykkedes indsættelsen skal funktionen returnerer '0'. Ved fejl '-1'.
*/
int wqueue_insert(wqueue_t *wq, unsigned int pri, work_f func, void *data) {
  // allocate space for job
  job *w = malloc(sizeof(job));
  if (w == NULL)
    return -1;
  
  // create job
  w.func = func;
  w.data = data;
  
  // insert job into pqueue
  return pqueue_insert(wq, pri, w);
}

/*
Udfører et stykke arbejde fra køen.

Hvis køen er tom returneres '0', ellers '1'.
*/
int wqueue_run(wqueue_t *wq) {
  // get job from pqueue
  job *w = pqueue_remove(wq);
  if (w == NULL)
    return 0;
  
  // fetch function and data from job
  work_f *func = w->func;
  void *data = w->data;
  
  // run function
  func(data);
  
  return 1;
}

main(){return 1;}