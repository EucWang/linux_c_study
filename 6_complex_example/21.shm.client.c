/**
  handle data
  client program
  */

#include "21.myshm.h"
#include "../test.h"

int complex_shm_client(){

    char *shm;
    int shmid;
    int producer, consumer , i;

    //create and init semaphore 'consumer'
    key_t tmp_key = ftok("consumer", 0);
    consumer = semget(tmp_key, 1, IPC_CREAT | 0660);
    if(consumer == -1){
        printf("semget failed.\n");
        exit(1);
    }
    //init 'consumer' semaphore's value one.
    init_a_semaphore (consumer, 0, 1);

    //create and init semaphore 'producer'
    tmp_key = ftok("producer", 0);
    producer = semget(tmp_key, 1, IPC_CREAT | 0660);
    if(producer == -1){
        printf("semget failed.\n");
        exit(1);
    }
    //init 'producer' semaphore's value one.
    init_a_semaphore (producer, 0, 1);

    //create and attach shared memory named 'shared'
    tmp_key = ftok("shared", 0);
    shmid = shmget(tmp_key, SHMSZ, 0660 | IPC_CREAT);
    if(shmid == -1){
        printf("shmget failed.\n");
        exit(1);
    }
    //attach
    shm = shmat(shmid, (unsigned char *)0, 0);
    if(shm == (void *)-1){
        printf("shmat failed\n");
        exit(1);
    }

    //read from shared memroy data which writes from service,
    //then output it
    for(i = 0; ; i++){
        semaphoreV (consumer); //return resources of consumer
        semaphoreP (producer); //use resources of producer, if producer is using, then wait

        //output shared memory's msg to stdin
        printf("Data received:%s\n", shm);

        //handle data ,if "end" then over the loop
        sleep(1);

        //how to end
        if(strstr(shm, "end") != NULL){
            break;
        }
    }

    //delete the two semophores
    semctl(producer, 0, IPC_RMID, 0);
    semctl(consumer, 0, IPC_RMID, 0);
}
