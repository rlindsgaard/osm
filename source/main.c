#include <stdlib.h>
#include <stdio.h>
#include "pqueue.h"
#include "wqueue.h"

int main(){
  pqueue_test();
 pqueue_t * wq;
printf("-------------------------------\n");

 wq = malloc(sizeof(pqueue_t));
 wq->e = NULL;

printf("wqueue tests:\n");
wqueue_add(wq,2,3,3);
wqueue_add(wq,5,8,2);
wqueue_add(wq,4,3,4);
pqueue_print(wq);
printf("Expect: %d ",13);
wqueue_run(wq);
printf("Expect: %d ",5);
wqueue_run(wq);
printf("Expect: %d ",7);
wqueue_run(wq);

pqueue_print(wq); //an empty list
printf("-------------------------------\n");
}
