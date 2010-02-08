#include <stdlib.h>
#include <stdio.h>
#include "pqueue.h"

int main()
{
 pqueue_t * pq;
 printf("inside main");
 pq = malloc(sizeof(pqueue_t));
// pq->e = NULL;
 int one = 1;
 int two = 2;
 int three = 3;
 printf("calling pqueue_insert");
 pqueue_insert(pq,1,&one);
 pqueue_insert(pq,3,&three);
// pqueue_insert(pq,3,&two);
 pqueue_print(pq);

}
