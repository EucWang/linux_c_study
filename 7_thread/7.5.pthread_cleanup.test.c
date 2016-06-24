/***
 * cancel a thread and the function of the canceled thread
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <errno.h>
#include "../test.h"

#define NUM_THREADS 5

static void *search(void *);

static void print_it(void *);

pthread_t threads[NUM_THREADS];

pthread_mutex_t lock;

int tries;

int started;

int pthread_cleanup_test(){
    int i, pid;

    pid = getpid();

    printf("Searching for the number = %d ...\n", pid);

    pthread_mutex_init (&lock, NULL);

    //create thread array
    for(started = 0; started < NUM_THREADS; started++){
        pthread_create (&threads[started], NULL, search, (void *)pid);
    }

    //waiting for the thread array to exit
    for(i = 0 ; i < NUM_THREADS; i++){
        pthread_join (threads[i], NULL);
    }

    pthread_mutex_destroy (&lock);
    return 0;
}

void *search(void *arg){
    int num = (int)arg;
    int i, j, ntries;

    pthread_t tid;

    tid = pthread_self ();  //current thread id

    //#define EBUSY 16 /* Device or resource busy */
    //when current thread canceled before it start run,
    //then current thread is step to below code
    //it try to lock and lock fail ,and the call function pthread_mutex_trylock() return EBUSY
    //mean it can response the cancel request,
    //call pthread_testcancel() to set the cancel point
    //and then thread canceled.
    while(pthread_mutex_trylock (&lock) == EBUSY) {
        printf("Thread %lx set cancel point and can be canceled.\n", tid);
        pthread_testcancel (); //set the cancel point
    }

    //set random seed
    srand((int)tid);

    i = rand() & 0xFFFFFF;
    printf("Thread %lx search begin with %d\n", tid, i);

    pthread_mutex_unlock (&lock);

    //enable thread cancel
    pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
    //set thread cancel defer
    pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, NULL);

    while(started < NUM_THREADS){
        printf("Thread %lx call sched_yield()", tid);
        sched_yield (); //priority down, other thread run, current thread wait
    }    
    pthread_cleanup_push(print_it , (void *)&ntries);

    ntries = 0;
    while(1){
        i = (i + 1) & 0xffffff;
        ntries ++;

        if(num == i){
            while(pthread_mutex_trylock (&lock) == EBUSY){
                printf("Thread %lx call pthread_testcancel() in loop to set cancel point.\n", tid);
                pthread_testcancel ();
            }

            tries = ntries;
            printf("Thread %lx found the number num = %d, i = %d, ntries = %d\n", tid, num, i, ntries);

            //cancel all other threads
            for(j = 0; j < NUM_THREADS; j++){
                if(threads[j] != tid){
                    pthread_cancel(threads[j]);
                }
            }
            break;
        }

        //every 100 times, try to set cancel point.
        if(ntries % 100 == 0){
//             printf("Thread %lx found the number num = %d, i = %d, ntries = %d\n", tid, num, i, ntries);
            pthread_testcancel ();
        }
    }

    pthread_cleanup_pop(0);
    return ((void *)0);
}


void print_it (void *arg){
    int *try = (int *)arg;
    pthread_t tid;
    tid = pthread_self ();
    printf("Thread %lx was canceled on its %d try.\n", tid, *try);
}
