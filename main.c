#include <stdio.h>
#include "test.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void thread_server_test(char *msg){
    printf("thread server start.\n");
    socket_server_test ();
    printf("thread server end.\n");
    return;
}

void thread_client_test(char *msg){
    printf("thread client start.\n");
    socket_client_test ();
    printf("thread client end.\n");
    return;
}


void thread_server_test2(char *msg){
    printf("thread server start.\n");
    recv_oob_test ();
    printf("thread server end.\n");
    return;
}

void thread_client_test2(char *msg){
    printf("thread client start.\n");
    send_oob_test ();
    printf("thread client end.\n");
    return;
}

void thread_cs_test(int flag){

    pthread_t pthread1, pthread2;
    int retval = 0;
    if(flag == 0){
        retval += pthread_create(&pthread1, NULL, (void *)(&thread_server_test), NULL);
    }else if(flag == 1){
         retval += pthread_create(&pthread1, NULL, (void *)(&thread_server_test2), NULL);
    }
    if(retval != 0){
        fprintf(stderr, "create server thread fail.\n");
        exit(1);
    }

    sleep(1);
    if(flag == 0){
        retval += pthread_create(&pthread2, NULL, (void *)(&thread_client_test), NULL);
    }else if(flag == 1){
        retval += pthread_create(&pthread2, NULL, (void *)(&thread_client_test2), NULL);
    }
    if(retval != 0){
        fprintf(stderr, "create client thread fail.\n");
        exit(1);
    }
    void *msg;
    pthread_join (pthread1, &msg);
    pthread_join (pthread2, &msg);
}

void shm_cs_test(char* argv[], int argc)
{
    if(argc != 2){
        printf("usage %s <server/client>\n", argv[0]);
        exit(0);
    }
    if(strcmp (argv[1], "server") == 0){
        shm_server();
    }else if(strcmp (argv[1], "client") == 0){
        shm_client();
    }else {
        printf("wrong second second arg. input 'server' or 'client'\n");
    }
}

/**
 * @brief shm_sem_cs_test use semaphore to control the shared memory
 * @param argv
 * @param argc
 */
void shm_sem_cs_test(char* argv[], int argc)
{
    if(argc != 2){
        printf("usage %s <server/client>\n", argv[0]);
        exit(0);
    }
    if(strcmp (argv[1], "server") == 0){
        complex_shm_server ();
    }else if(strcmp (argv[1], "client") == 0){
        complex_shm_client();
    }else {
        printf("wrong second second arg. input 'server' or 'client'\n");
    }
}

int main(int argc, char* argv[])
{
//    semop_test(argc, argv);

    //test share memory
//    shm_cs_test(argv, argc);

//    shm_addr_test ();

//    shm_sem_cs_test(argv, argc);

//    pthread_create_test ();

//    pthread_join_test();
//    pthread_mutex_test ();

//    pthread_cond_wait_test ();

//    pthread_cleanup_test();

//    pthread_specific_test();

//    pthread_attr_test ();

//    net_bite_sequence_test();

//    server_test();

//    thread_cs_test(0);

    thread_cs_test(1);

    return 0;
}
