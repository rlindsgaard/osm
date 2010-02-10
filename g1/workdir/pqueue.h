#define PQUEUE_INIT {NULL}

/*
�ben definition af en prioritetesk� instans. Denne kan
�ndres efter behov, s�l�nge det ikke �ndrer p� nedenst�ende
funktioners gr�nseflade.
*/
typedef struct pqueue pqueue_t;

typedef struct entry entry_t;

struct pqueue {
    entry_t *e;
};

/*
Inds�tter et elementet som 'elem' peger p�, med prioriteten 'pri'.
Det er muligt at inds�tte flere elementer med samme prioritet.
'pq' peger p� en instans af prioritetsk�en.

Lykkedes inds�ttelsen skal funktionen returnerer '0'. Ved fejl '-1'.
*/
int pqueue_insert(pqueue_t *pq, unsigned int pri, void *elem);


/*
Returnerer det element som har h�jst prioritet. Hvis flere elementer
har samme h�jeste prioritet returneres det af dem, som f�rst blev indsat.

Hvis k�en er tom, returneres 'NULL'.
*/
void *pqueue_remove(pqueue_t *pq);

void print_struct(entry_t *p,int line);

#include "pqueue.c"
