#include <stdlib.h>
#include <stdio.h>
#include "pqueue.h"

int main(){
 pqueue_t * pq;
 pq = malloc(sizeof(pqueue_t));
 pq->e = NULL;
 pq->counter = 0;
 int one = 1;
 int two = 2;
 int three = 3;
 int four = 4;
 int five = 5;
 int six = 6;

 printf("Removing from an empty queue. Result: %d\n",pqueue_remove(pq));
 printf("Queueing nothing. Result: %d\n",pqueue_insert(NULL,1,&one));
 printf("Inserting null element. Result: %d\n",pqueue_insert(pq,1,NULL));
 
 //building queue
 pqueue_insert(pq,1,&one);
 pqueue_insert(pq,2,&two);
 pqueue_insert(pq,3,&three);
 pqueue_insert(pq,3,&four);
 pqueue_insert(pq,5,&five);
 pqueue_print(pq);
 
 //checking fifo
 pqueue_insert(pq,3,&six);
 pqueue_print(pq);

 //removing in the right order
 pqueue_remove(pq);
 pqueue_print(pq);
 pqueue_remove(pq);
 pqueue_print(pq);


}
