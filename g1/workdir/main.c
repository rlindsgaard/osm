#include <stdlib.h>
#include <stdio.h>
#include "pqueue.h"
#include "wqueue.h"

int main(){
 pqueue_t * pq;
 pqueue_t * wq;
 pq = malloc(sizeof(pqueue_t));
 pq->e = NULL;
 pq->counter = 0;

printf("-------------------------------\n");
printf("pqueue tests\n");
 int one = 1;
 int two = 2;
 int three = 3;
 int four = 4;
 int five = 5;
 int six = 6;

 //building queue
 pqueue_insert(pq,1,&one);
 pqueue_insert(pq,2,&two);
 pqueue_insert(pq,3,&three);
 pqueue_insert(pq,3,&four);
 pqueue_insert(pq,5,&five);
 printf("expecting:\n1,2,3,4,5\n");
 pqueue_print(pq);
 
 //checking fifo

 pqueue_insert(pq,3,&six);
 printf("expecting:\n1,2,3,4,6,5\n");
 pqueue_print(pq);

 //removing in the right order
 pqueue_remove(pq);
 printf("expecting:\n2,3,4,6,5\n");

 pqueue_print(pq);
 printf("expecting:\n3,4,6,5");
 pqueue_remove(pq);
 pqueue_print(pq);

printf("-------------------------------\n");

 wq = malloc(sizeof(pqueue_t));
 wq->e = NULL;
 wq->counter = 0;

printf("pqueue tests:\n");
wqueue_test(wq,2,3,3);
wqueue_test(wq,5,8,2);
wqueue_test(wq,4,3,4);
pqueue_print(wq);
printf("Expect: %d ",13);
wqueue_run(wq);
printf("Expect: %d ",5);
wqueue_run(wq);
printf("Expect: %d ",7);
wqueue_run(wq);
pqueue_print(wq);
printf("-------------------------------\n");
}
