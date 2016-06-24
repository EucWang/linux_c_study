/***
 * thread specific data handle function test
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../test.h"

#if 0
char *str_accumulate(char *s){
    static char accu[1024] = {0};
    strcat(accu, s);
    return accu;
}
#endif

static pthread_key_t str_key;
static pthread_once_t str_alloc_key_once = PTHREAD_ONCE_INIT;
static void str_alloc_key();
static void str_alloc_destroy_accu(void *accu);

char *str_accumulate(const char *s){
    char *accu;

    //init a data once
    pthread_once(&str_alloc_key_once, str_alloc_key);

    accu = (char *)pthread_getspecific(str_key);

    if (accu == NULL){
        accu = malloc(1024);
        if(accu == NULL){
            return NULL;
        }
        accu[0] = 0;
        pthread_setspecific(str_key, (void *)accu);
        printf("Thread %lx :allocating buffer at %p\n", pthread_self (), accu);
    }

    strcat(accu, s);
    return accu;
}

static void str_alloc_key(){
    pthread_key_create (&str_key, str_alloc_destroy_accu);
    printf("Thread %lx : allocated key %d\n", pthread_self(), str_key);
}

static void str_alloc_destroy_accu (void *accu){
    printf("Thread %lx : freeing buffer at %p\n", pthread_self (), accu);
    free(accu);
}

/**
 * @brief process thread run function
 * @param arg
 * @return
 */
void *process(void *arg){
    char *str;
    str =str_accumulate ("Result of ");
    str = str_accumulate ((char *)arg);
    str = str_accumulate (" thread");
    printf("Thread %lx:\"%s\"\n",pthread_self (), str);
    return NULL;
}

int pthread_specific_test(){
    char *str;
    pthread_t th1, th2;

    str = str_accumulate ("Result of ");

    //create two thread
    pthread_create (&th1, NULL, process,  (void *)"first");
    pthread_create(&th2, NULL, process, (void *)"second");

    str = str_accumulate ("initial thread ");
    printf("Thread %lx :\"%s\"\n",pthread_self (), str);
    pthread_join (th1, NULL);
    pthread_join(th2, NULL);
    return 0;
}
