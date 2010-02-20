/* vim: set sw=4 sts=4 et foldmethod=syntax : */

#ifndef GUARD_RING_H
#define GUARD_RING_H 1

#define NUM_THREADS 5
#define NUM_RUNS 5

//Define the global variables for use by the threads
pthread_mutex_t locks[NUM_THREADS];

//Define structures and types
typedef struct data data_t;

struct data {
    int tid;
    int next;
    int *baton;
};
/* The regular threads in the chain performing stuff */
void * link(void * data);
/* A special link able to shutdown the chain */
void * padlock(void * data);

void * worker(void * data);

#endif
