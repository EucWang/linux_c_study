/**
 * in program, create two threads,
 * one print slef's thread id and 'hello'
 * two print slef's id and 'world'
 *
 ****
 **** use gcc to compile, add args '-lpthread'
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../test.h"

void print_msg(char *ptr);

int pthread_create_test(){

    pthread_t thread1, thread2;

    char *msg1 = "hello\n";
    char *msg2 = "world\n";

    //create two thread
    pthread_create (&thread1, NULL, (void *)(&print_msg), (void *)msg1);
    pthread_create (&thread2, NULL, (void *)(&print_msg), (void *)msg2);
    sleep(1);
    return 0;
}

void print_msg (char *ptr){
    int retval;

    //get the current thread id and then output it
    printf("Thread ID :%lx", pthread_self ());
    printf("%s", ptr);

    //current thread exit
    pthread_exit (&retval);
}
