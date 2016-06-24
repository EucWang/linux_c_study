#ifndef TEST_H_INCLUDE
#define TEST_H_INCLUDE

////////6.ipc///////////////

/**
  信号量最常见的用法是控制程序中的一个关键区.
  这个关键区可能需要访问由多个进程共享的数据段，也可能需要访问其他共享资源

  输出命令行参数，每个参数输出一行，每输出一个字符，需要随机睡眠几秒
  如果同时有两个进程执行它，那么就有可能导致同一行的输出来自不同的进程
  为了保证每一行的输出内容来自同一个进程，完成一个参数输出工作的代码必须作为关键区
 */
int semop_test(int argc, char* argv[]);

int shm_client();

int shm_server();

int shm_addr_test();

int complex_shm_client();

int complex_shm_server();

////////////7.thread//////////////////

int pthread_create_test();

int pthread_join_test();


int pthread_mutex_test();


int pthread_cond_wait_test();


int pthread_cleanup_test();


int pthread_specific_test();

int pthread_attr_test();


/////////////8.network/////////////////////////


int net_bite_sequence_test();

int server_test();

int socket_server_test();

int socket_client_test();

int recv_oob_test();
int send_oob_test();
#endif

