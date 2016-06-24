#include "../test.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

static int semaphore_P(int);

static int semaphore_V(int sem_id);

/**
  信号量最常见的用法是控制程序中的一个关键区.
  这个关键区可能需要访问由多个进程共享的数据段，也可能需要访问其他共享资源
  输出命令行参数，每个参数输出一行，每输出一个字符，需要随机睡眠几秒
  如果同时有两个进程执行它，那么就有可能导致同一行的输出来自不同的进程
  为了保证每一行的输出内容来自同一个进程，完成一个参数输出工作的代码必须作为关键区

  terminate command:
     ./linux_study 1 22 333 4444 & /*add & means backstage run program
 */

int semop_test(int argc, char* argv[]){
    int sem_id;
    int i, creat = 0;
    int pause_time;
    char *cp;

    //terminate input args, args can not be null
    if(argc <= 1){
        printf("usage:%s parameter1 parameter2 ...\n", argv[0]);
        exit(1);
    }

    //the pid of this process as the seed for random number
    srand((unsigned int)getpid());

    //get or create the semaphore(信号量)
    sem_id = semget((key_t)1234, 1, IPC_CREAT|0660);
    if(sem_id == -1){
        printf("semget failed.\n");
        exit(2);
    }

    //if the first args is "1", then set the first semaphore'value "1"
    //when the first arg is "1", this process as the main process
    if(strcmp(argv[1], "1")){
        semctl(sem_id, 0, SETVAL, 1);
        creat = 1;
        sleep(2);
    }

    //traversal all args
    for(i = 0; i<argc; i++){
        cp = argv[i]; //item for each args
        if(!semaphore_P (sem_id)){
            exit(3);
        }

        //////////////below's is the key zone//////
        printf("process %d:", getpid());
        fflush(stdout);

        while(*cp){
            printf("%c", *cp);
            fflush(stdout);
            pause_time = rand() % 3;
            sleep(pause_time);
            cp++;
        }

        printf("\n");
        //////////up's is the key zone////////////
        if(!semaphore_V (sem_id)){
            exit(4);
        }

        pause_time = rand() % 2;
        sleep(pause_time);
    }

    printf("\n %d - finished\n", getpid());

    //delete semophore ,must delete it
    if(creat == 1){
        sleep(10);
        semctl(sem_id, 0, IPC_RMID, 0);
    }

    return 0;
}

int semaphore_P (int sem_id){
    struct sembuf sb;
    sb.sem_num = 0;       //index of the semaphore in the semahpore collection
    sb.sem_op = -1;       //control flag : negative integer means use resource
    sb.sem_flg = SEM_UNDO;  //resever the value

    //semop function control semaphore
    //first arg:  id of semaphore
    //second arg: array of struct sembuf
    //third arg:  array's length
    if(semop(sem_id, &sb, 1) == -1){
        printf("semaphore_P failed.\n");
        return 0;
    }

    return 1;
}

int semaphore_V (int sem_id){
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;        //control flag : positive integer means return resource
    sb.sem_flg = SEM_UNDO;

    if(semop(sem_id, &sb, 1) == -1){
        printf("semaphore_V failed.\n");
        return 0;
    }

    return 1;
}
