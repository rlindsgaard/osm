#include <stdlib.h>
#include <stdio.h>
#include "pqueue.h"

int pqueue_t()
{
 pqueue_t * pq;
 pq = malloc(sizeof(pqueue_t));
 int one = 1;
 int two = 2;
 int three = 3;
 int four = 4;
 int five = 5;
 int six = 6;

 pqueue_insert(pq,1,&one);
 pqueue_insert(pq,2,&two);
 pqueue_insert(pq,3,&three);
 pqueue_insert(pq,3,&four);
 pqueue_insert(pq,5,&five);
 pqueue_print(pq);

 printf("We are now ready to make errors\n");

 pqueue_insert(pq,3,&six);
 pqueue_print(pq);
 pqueue_remove(pq);
 pqueue_remove(pq);
 pqueue_insert(pq,1,&one);
 pqueue_print(pq);

}
