#ifndef WQUEUE_GUARD
#define WQUEUE_GUARD 1

#include "pqueue.h"

#define WQUEUE_INIT PQUEUE_INIT

/*
Definition af arbejdsfunktionen.
*/
typedef void (*work_f)(void *);

/*
wqueue_t er simpelthen bare en prioritetsk�, som indeholder
arbejdsopgaver.
*/
typedef pqueue_t wqueue_t;

typedef struct job job_t;

struct job {
  work_f func;
  void * data;
};

/*
Inds�tter et stykke arbejde i form af en funktion 'func' og det
data 'data' den skal kaldes med. Prioriteten 'pri' angiver
arbejdets prioritet. 'wq' peger p� en instans af arbejdsk�en.

Lykkedes inds�ttelsen skal funktionen returnerer '0'. Ved fejl '-1'.
*/
int wqueue_insert(wqueue_t *wq, unsigned int pri, work_f func, void *data);

/*
Udf�rer et stykke arbejde fra k�en.

Hvis k�en er tom returneres '0', ellers '1'.
*/
int wqueue_run(wqueue_t *wq);

void print_data(void *s);

void wqueue_add(wqueue_t *wq, int a, int b, int pri);
#endif //WQUEUE_GUARD
