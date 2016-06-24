

#include "21.myshm.h"
#include "../test.h"

int complex_shm_server(){
    char *shm, *s;
    int shmid;
    int producer, consumer, i;
    char readbuf[SHMSZ];

    //create and init semophore 'consumer'
    key_t tmp_key = ftok("consumer", 0);
    consumer = semget(tmp_key, 1, IPC_CREAT|0660);
    if(consumer == -1){
        printf("semget failed.\n");
        exit(1);
    }
    //init set consumer's semophore value is one
    init_a_semaphore (consumer, 0 ,1);

    //create and init semophore 'producer'
    tmp_key = ftok("producer", 0);
    producer = semget(tmp_key, 1, IPC_CREAT|0660);
    if(producer == -1){
        printf("semget failed.\n");
        exit(1);
    }
    //init set producer's semophore value is one
    init_a_semaphore (producer, 0 ,1);

    //create or get the shared memory named 'shared'
    tmp_key =  ftok("shared", 0);
    shmid = shmget(tmp_key, SHMSZ, 0660|IPC_CREAT);
    if(shmid == -1){
        printf("shmget failed.\n");
        exit(1);
    }
    //attach
    shm = shmat(shmid, (unsigned char *)0 , 0);
    if(shm == (void *)-1){
        printf("shmat failed.\n");
        exit(1);
    }

    //read from stdin and write data to shared memory
    for(i = 0; ; i++){
        printf("Enter text:");
        fgets(readbuf, SHMSZ, stdin);

        //trim at the end ofstring with '\r\n'
        if(readbuf[strlen(readbuf)-1] == '\n'){
            readbuf[strlen(readbuf)-1] = '\0';
        }

        semaphoreP (consumer);  //use resource of consumer, if consumer is using, then block program and  waiting
        //output msg to the shared memory
        sprintf(shm, "Message %4d from producer %d is \'%s\'. \n", i, getpid(), readbuf);
        semaphoreV (producer);   //return resource of producer

        //how to end loop
        if(strcmp (readbuf, "end") == 0){
            break;
        }

    }
    return 0;
}
