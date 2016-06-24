/**
使用共享内存交换数据对一个例子

一个程序是数据处理程序，
另一个是数据生成程序
这两个程序使用两个信号量 consumer 和producer来同步他们之间对共享内存的访问。
cosumer信号量指出共享存储段中的数据是否已被处理完毕。
只有当处理完毕后才允许数据生成程序在其中继续生成数据。
producer指出数据是否已生成在共享内存，一个共享内存用来交换数据，
所交换数据的格式由exchange结构描述。
其中成员buf用来存放数据，seq存放客户进程写入的顺序号，
两个程序都使用一个指向该共享存储段的指针对共享内存直接进行读写。
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SHMSZ 256

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void init_a_semaphore(int sid, int semnum, int initval);

int semaphoreP(int sem_id);

int semaphoreV(int sem_id);
