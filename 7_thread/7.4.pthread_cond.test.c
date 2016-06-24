/***
 * a typical program about producer and consumer
 */

#include <stdio.h>
#include <pthread.h>
#include "../test.h"

#define BUFFER_SIZE 4
#define OVER (-1)

struct producers{
    int buffer[BUFFER_SIZE];  //buffer array
    pthread_mutex_t lock;
    int readpos, writepos;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
};

/**
 * @brief init  init the producers' buffer
 * @param b
 */
static void init(struct producers *b){
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init (&b->not_empty, NULL);
    pthread_cond_init (&b->not_full, NULL);
    b->readpos = 0;
    b->writepos = 0;
}


static void put(struct producers *b, int data){
    pthread_mutex_lock (&b->lock);

    while((b->writepos + 1) % BUFFER_SIZE == b->readpos){
        //current thread waiting
        printf("Producer wait...\n");
        pthread_cond_wait(&b->not_full, &b->lock);
    }

    //write data and move the point
    b->buffer[b->writepos] = data;
    b->writepos++;
    if(b->writepos >= BUFFER_SIZE){
        b->writepos = 0;
    }

    //send signal about buffer having data
    pthread_cond_signal (&b->not_empty);
    pthread_mutex_unlock (&b->lock);
}

/**
 * @brief get get data from buffer and remove data from buffer
 * @param b
 * @return
 */
static int get(struct producers *b){
    int data;

    pthread_mutex_lock (&b->lock);

    while(b->writepos == b->readpos){
        //current thread waiting.
        printf("Consumer wait...\n");
        pthread_cond_wait (&b->not_empty, &b->lock);
    }

    //read data and move the read point
    data = b->buffer[b->readpos];
    b->readpos++;
    if(b->readpos >= BUFFER_SIZE){
        b->readpos = 0;
    }

    //send signal buffer is not full
    pthread_cond_signal (&b->not_full);

    pthread_mutex_unlock (&b->lock);

    return data;
}

struct producers buffer;

/**
 * @brief producer thread function to produce
 * @param data
 * @return
 */
static void *producer(void *data){
    int n;
    for(n = 0; n < 10; n++){
        printf("Producer : %d-->\n", n);
        put(&buffer, n);
        printf("Producer :%d-->success\n", n);
    }
    put(&buffer, OVER);
    return NULL;
}

/**
 * @brief consumer  thread function to consume
 * @param data
 * @return
 */
static void *consumer(void *data){
    int d;
    while(1){
        printf("Consumer: -->start get\n", d);
        d = get(&buffer);
        if(d == OVER){
            printf("Consumer: --> %d-->over\n", d);
            break;
        }
        printf("Consumer: --> %d-->success\n", d);
    }
    return NULL;
}

/**
 * @brief pthread_cond_wait_test main test to enter
 * @return
 */
int pthread_cond_wait_test(){
    pthread_t tha, thb;
    void *retval;
    init(&buffer);
    //create two thread
    pthread_create (&tha, NULL, producer, 0);
    pthread_create (&thb, NULL, consumer, 0);

    //wait for two thread exit and then main thread exit
    pthread_join (tha, &retval);
    pthread_join (thb, &retval);
    return 0;
}



