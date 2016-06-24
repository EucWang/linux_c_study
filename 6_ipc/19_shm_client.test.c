/**
 * share memory
 * client mode
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../test.h"

int shm_client(){

    int shmid;
    char c;
    char *shmptr, *s;

    //get the share memory
    //flag do not set IPC_CREAT, then if server do not run, current process return fail.
    shmid = shmget(1235, 256, 0666);
    if(shmid < 0){
        printf("shmget failed.\n");
        exit(1);
    }

    //attach the share memory to current process
    //get addr of the share memory
    shmptr = shmat(shmid, 0, 0);
    if(shmptr == (void *)(-1)){
        shmctl (shmid, IPC_RMID, NULL);
        printf("shmat failed.\n");
        exit(2);
    }

    //traveral all share memory
    //console output share memory content
    for(s = shmptr; *s != NULL; s++){
        putchar(*s);
    }
    printf("\n");

    //set share memory
    //to notificate the server process to end program
    *shmptr = '*';
    return 0;
}
