/**
 * use share memory to realization server-client connection
 *
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../test.h"

int shm_server(){
    int shmid;
    char c;
    char *shmptr, *s;

    //create or get the share memory
    //first arg:  share memory id
    //second arg: the size of share memory
    //third arg:  flag to create or get
    shmid = shmget(1235, 256, IPC_CREAT|0660);
    if(shmid < 0){
        printf("shmget failed.\n");
        exit(1);
    }

    //attach the share memory to current process
    //first arg: id of share memory
    //second arg:  zero(0) mean auto attach share memory to current process memory
    //third arg:   when second arg is 0, then do not need the third arg
    shmptr = shmat(shmid, 0 , 0);
    //attach fail, need to delete the share memory
    if(shmptr == (void *)-1){
        //remoe share memory
        shmctl (shmid, IPC_RMID, NULL);
        printf("shmat failed.\n");
        exit(2);
    }

    s = shmptr;
    //with english letter to fill the share memory
    for(c = 'a'; c <= 'z'; c++){
        *s ++ = c;
    }

    *s = NULL;
    //wait for the client process set the first addr of share memory to '*'
   printf("set share memory done. then waiting for client program.\n");
    while(*shmptr != '*'){
        sleep(1);
    }
  printf("wait end. delete share memory.\n");
    //delete the share memory
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
