/**
  *** int main thread
  *** create two children thread
  *** and then main thread hang up and waiting two children thread return
  ***/

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "../test.h"

static void print_msg(char *ptr);

int pthread_join_test(){
    pthread_t thread1, thread2;

    int i, j;

    void *retval;
    char *msg1 = "Hello\n";
    char *msg2 = "World\n";

    pthread_create(&thread1, NULL, (void *)(&print_msg), (void *)msg1);
    pthread_create(&thread2, NULL, (void *)(&print_msg), (void *)msg2);

    pthread_join (thread1, &retval);
    pthread_join (thread2, &retval);
    return 0;
}

void print_msg(char *ptr){
    int i;
    for(i = 0; i < 10000; i++){
        printf("%s", ptr);
    }
}


