#include "21.myshm.h"
/**
 * @brief init_a_semaphore
 * @param sid     : id of semaphore
 * @param semnum  : index of a semaphore in semaphore collection
 * @param initval : value of a semaphore
 */
void init_a_semaphore(int sid, int semnum, int initval){
    union semun semopts;
    semopts.val = initval;
    /**
     * @brief semctl    set or control a semaphore
     * @param sid       id of semaphore
     * @param semnum    index
     * @param SETALL    all semaphore collection's elements
     * @param semopts   have the value of a semaphore
     */
    semctl(sid, semnum, SETALL, semopts);
}
/**
 * @brief semaphoreP  use resource of semaphore
 * @param sem_id  id of semaphore
 * @return 1/0, 1 mean success, 0 fail
 */
int semaphoreP(int sem_id){
    struct sembuf sb;
    sb.sem_num = 0;    // indexo of semaphore
    sb.sem_op = -1;    // -1  use resources
    sb.sem_flg = SEM_UNDO;  // flag

    //handle semophore
    if(semop(sem_id, &sb, 1) == -1){
        printf("semaphoreP failed.\n");
        return 0;
    }
    return 1;
}

/**
 * @brief semaphoreV :return resources of semaphore
 * @param sem_id :id of semaphore`
 * @return 0/1, 0fail,1success
 */
int semaphoreV(int sem_id){
    struct sembuf sb;
    sb.sem_num = 0;      //index of semaphore
    sb.sem_op = 1;       // return resources
    sb.sem_flg = SEM_UNDO;  //fixed flag

    //handle semaphore
    if(semop(sem_id, &sb, 1) == -1){
        printf("semaphoreV failed.\n");
        return 0;
    }

    return 1;
}

