#define PQUEUE_INIT {NULL}

/*
Åben definition af en prioriteteskø instans. Denne kan
ændres efter behov, sålænge det ikke ændrer på nedenstående
funktioners grænseflade.
*/
typedef struct pqueue pqueue_t;

typedef struct entry entry_t;

struct pqueue {
    entry_t *e;
};

/*
Indsætter et elementet som 'elem' peger på, med prioriteten 'pri'.
Det er muligt at indsætte flere elementer med samme prioritet.
'pq' peger på en instans af prioritetskøen.

Lykkedes indsættelsen skal funktionen returnerer '0'. Ved fejl '-1'.
*/
int pqueue_insert(pqueue_t *pq, unsigned int pri, void *elem);


/*
Returnerer det element som har højst prioritet. Hvis flere elementer
har samme højeste prioritet returneres det af dem, som først blev indsat.

Hvis køen er tom, returneres 'NULL'.
*/
void *pqueue_remove(pqueue_t *pq);

void print_struct(entry_t *p,int line);

#include "pqueue.c"
