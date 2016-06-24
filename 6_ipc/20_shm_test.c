/**
  when system call shmat
  and addr = 0,
  then share memory's addr decide by system.
  test the addr of share memory decided by system
  */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include "../test.h"

char array[4000];

int shm_addr_test(){
    int shmid;
    char *ptr, *shmptr;

    printf("array[] form %x to %x \n", &array[0], &array[3999]);
    printf("stack around %x\n", &shmid);

    ptr = malloc(10000);
    if(ptr == NULL){
        printf("malloc failed.\n");
        exit(1);
    }

    //create shared memory which addr decided by system
    shmid = shmget(IPC_PRIVATE , 10000, SHM_R | SHM_W);
    if(shmid < 0){
        printf("shmget failed.\n");
        exit(2);
    }

    //attach shared memory to  current process
    //and get the addr of shared memory
    shmptr = shmat(shmid, 0, 0);
    if(shmptr == (void *)(-1)){
        printf("shmat failed.\n");
        exit(3);
    }

    printf("shared memory attached from %x to %x\n", shmptr, shmptr - 10000);

    //delete shared memory
    if(shmctl (shmid, IPC_RMID, NULL) < 0){
        printf("shmctl failed.\n");
        exit(4);
    }
    return 0;
}
