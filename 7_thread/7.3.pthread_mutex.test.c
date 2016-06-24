/****
 * a simple read/write program
 * one thread read data from shared buffer
 * and another thread(main thread) write data to shared buffer
 * to control visiting the shared buffer, using mutex(互斥锁)
 *
 */

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define FALSE 0
#define TRUE 1

static void readfunc();
static void writefunc();

char buffer[256];
int buffer_has_item = 0;
int retflag = FALSE;
pthread_mutex_t mutex;

int pthread_mutex_test(){
    pthread_t reader;

    //init the mutex, and then get the mutex id
    pthread_mutex_init(&mutex, NULL);

    /**
     * @brief pthread_create create a thread and start the thread
     * @param reader thread id
     * @param NULL attr NULL
     * @param readfunc thread's function
     * @param NULL arg is NULL
     */
    pthread_create(&reader, NULL, (void *)&readfunc, NULL);
    writefunc();
    pthread_mutex_destroy (&mutex);
    return 0;
}

void readfunc (){
    while(TRUE){
        if(retflag){
            return;
        }
        pthread_mutex_lock (&mutex);
        if(buffer_has_item == 1){
            printf("%s", buffer);
            buffer_has_item = 0;
        }
        pthread_mutex_unlock (&mutex);
    }
}

void writefunc (){
    int i = 0;
    while(TRUE){
        if(i == 10){
            retflag = TRUE;
            return;
        }
        pthread_mutex_lock(&mutex);
        if(buffer_has_item == 0){
            sprintf(buffer, "This is %d\n", i++);
            buffer_has_item = 1;
        }
        pthread_mutex_unlock (&mutex);
    }
}


